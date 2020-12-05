#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "prog2.h"

/* ******************************************************************
ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR:

VERSION 1.1 by J.F.Kurose

This code should be used for PA2, unidirectional or bidirectional
data transfer protocols (from A to B. Bidirectional transfer of data
is not required for this assignment).  Network properties:
- one way network delay averages five time units (longer if there
are other messages in the channel for GBN), but can be larger
- packets can be corrupted (either the header or the data portion)
or lost, according to user-defined probabilities
- packets will be delivered in the order in which they were sent
(although some can be lost).

VERSION 2.0 by Siyuan
- enable multiple timer interrupts for the same side, for
implementing selective repeat
**********************************************************************/


/* ###########################################################################
 * #									                                                       #
 * #			ALTERNATING-BIT PROTOCOL		  	                                   #
 * #									                                                       #
 * #	- Stop-and-wait method						                                     #
 * #    - Unidirectional transfer (from side A to side B)	  	               #
 * #    - Uses ACK (no NAK)                                                  #
 * #    - Very large time for average time between messages (1000)           #	
 * #    - If there is a message in A_output being transmitted, DROP	         #
 * ###########################################################################
 */


/********* STUDENTS WRITE THE NEXT SIX ROUTINES *********/

#define A_SIDE 0
#define B_SIDE 1

int compute_checksum(pkt);

int curr_seqnum;        // Only accessed by A-side
int expected_acknum;    // Only accessed by A-side
bool in_transit;        // Only accessed by A-side
int expected_seqnum;    // Only accessed by B-side
struct pkt curr_pkt;    // A-side's current sending packet, used for retransmit

/* Called from layer 5, passed the data to be sent to other side */
void A_output(msg message) {
  printf("- A side received message from upper layer:\t%s\n", message.data);
  struct pkt new_pkt = { 0 };

  if (in_transit) {
    // Packet is still in transit. DROP
    printf("- Packet is still in transit - DROP PACKET\n");
    return;
  }

  // Set sequence number to the one that side B expects next
  new_pkt.seqnum = curr_seqnum;

  // We expect to get an ACK with ACKnum of seqnum + len(message)
  new_pkt.acknum = curr_seqnum;

  strncpy(new_pkt.payload, message.data, MESSAGE_LEN);
  new_pkt.checksum = compute_checksum(new_pkt);

  // Save a copy of the packet in case it needs to be retransmitted
  curr_pkt = new_pkt;

  // Start timer and send the packet
  printf("- TIMER STARTED\n");
  printf("- Sending Message:\t\t\t\t%s\tack# = %d\tseq# = %d\n", new_pkt.payload, new_pkt.acknum, new_pkt.seqnum);
  tolayer3(A_SIDE, new_pkt);
  starttimer(A_SIDE, TIMEOUT, NULL);
  in_transit = true;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(pkt packet) {
  /* TODO: packet is possibly corrupted, check CHECKSUM
   *       This function verifies that the packet is ACKED
   */

  printf("- A Side Received from Lower Layer:\t\t%s\t\t\tack# = %d\tseq# = %d\n", packet.payload,
      packet.acknum, packet.seqnum);

  if ((compute_checksum(packet) == packet.checksum) &&
      (packet.acknum == expected_acknum)) {
    // ACK packet arrived intact
    printf("- TIMER STOPPED\n");
    stoptimer(A_SIDE, NULL);
    printf("- A Side received correct packet\n\n\n");
    expected_acknum = !expected_acknum;
		curr_seqnum = !curr_seqnum;
    in_transit = false;
  }
  else if (compute_checksum(packet) != packet.checksum) {
    // The A side received a corrupted ACK from B
    printf("- A Side received corrupted packet from\n"
        "  B Side - resending:\t\t\t\t%s\tack# = %d\tseq# = %d\n", curr_pkt.payload,
				curr_pkt.acknum, curr_pkt.seqnum);
    tolayer3(A_SIDE, curr_pkt);
  }
}

/* Called when A's timer goes off - Retransmits packet */
void A_timerinterrupt(void *adata) {
  printf("- TIMER INTERRUPT (Packet Lost) -\n"
      "  Retransmitting & Restarting Timer:\t\t%s\tack# = %d\tseq# = %d\n", curr_pkt.payload,
			curr_pkt.acknum, curr_pkt.seqnum);
  tolayer3(A_SIDE, curr_pkt);
  starttimer(A_SIDE, TIMEOUT, NULL);
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  curr_seqnum = 0;
  expected_acknum = 0;
  in_transit = false;
}

/* called from layer 3, when a packet arrives for layer 4 at B */
void B_input(pkt packet) {
  /* TODO: Send ACK for packet. Verify that packet has arrived correctly and
   * 	   intact. Send packet to upper layer.
   */

  printf("- B side received packet:\t\t\t%s\tack# = %d\tseq# = %d\n",
      packet.payload, packet.acknum, packet.seqnum);

  struct pkt ack_pkt = { 0 };
  strncpy(ack_pkt.payload, "ACK", MESSAGE_LEN);

  if (compute_checksum(packet) == packet.checksum) {
    if (packet.seqnum == expected_seqnum) {
      // Valid packet - send normal ACK
      printf("- B side received correct packet\n");
      ack_pkt.acknum = packet.acknum;
      ack_pkt.seqnum = packet.acknum;
      expected_seqnum = !packet.acknum;

      // Also send message to layer 5
      printf("- Sending message to layer 5...\n");
      tolayer5(B_SIDE, packet.payload);
    }
    else {
      printf("- B side expected seq %d, got seq %d\n", expected_seqnum,
          packet.seqnum);
      ack_pkt.acknum = !expected_seqnum;
      ack_pkt.seqnum = !expected_seqnum;
    }
  }
  else {
    // Packet is corrupted - checksum failed
    printf("- B side received corrupted packet. Ignoring...\n");
		return;
  }

  // Send ACK to A side
  printf("- B side sent ACK:\t\t\t\t%s\t\t\tack# = %d\tseq# = %d\n", ack_pkt.payload,
      ack_pkt.acknum, ack_pkt.seqnum);
  ack_pkt.checksum = compute_checksum(ack_pkt);
  tolayer3(B_SIDE, ack_pkt);
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init() {
  expected_seqnum = 0;
}

/* Computing a checksum similar to TCP's checksum:
 * checksum = sequence_num + ack_num + (char by char sum of payload)
 */
int compute_checksum(pkt packet) {
  int checksum = 0;
  checksum += packet.seqnum + packet.acknum;
  for (int i = 0; i < MESSAGE_LEN; i++) {
    checksum += (int) packet.payload[i];
  }

  return checksum;
}
