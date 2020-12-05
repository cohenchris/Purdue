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


/* ############################################################################
 * #									                                                        #
 * #			                    GO-BACK-N PROTOCOL		  	     	                #
 * #									                                                        #
 * ############################################################################
 */


/********* STUDENTS WRITE THE NEXT SIX ROUTINES *********/

#define A_SIDE 0
#define B_SIDE 1
#define WINDOW 8
#define MAXBUFF 100

extern float time;

int compute_checksum(pkt);
struct sending create_pkt(int, int, char *);

// A SIDE
typedef struct sending {
  struct pkt packet;
  float start_time;
  bool sent;
} sending;

struct sending sndpkt[MAXBUFF]; // A-side's packets that it's sending
int nextseqnum;                 // Next packet sequence number to send
int base;                       // Beginning of sender window

// B SIDE
int expected_seqnum;

/* called from layer 5, passed the data to be sent to other side */
void A_output(msg message) {
  /* TODO: Ensure that the message (from upper layer) is delivered
   * in-order, and correctly to the receiving side upper layer.
   */

  // JUST FOR READABILITY
  message.data[MESSAGE_LEN - 1] = '\0';

  printf("- A side received message from upper layer:\t%s\n", message.data);

	if (nextseqnum >= MAXBUFF) {
	  printf("**********     ERROR     **********\n"
					 "Sender has run out of buffer space.\n"
					 "Terminating the program.\n"
	  			 "**********     ERROR     **********\n");
		exit(EXIT_FAILURE);
	}

  // Create packet
  sndpkt[nextseqnum] = create_pkt(nextseqnum, nextseqnum, message.data);

  if (nextseqnum < base + WINDOW) {
    // Next packet is within the sending window
    printf("- Sending Message:\t\t\t\t%s\tack# = %d\tseq# = %d\n",
        sndpkt[nextseqnum].packet.payload, sndpkt[nextseqnum].packet.acknum,
        sndpkt[nextseqnum].packet.seqnum);
    tolayer3(A_SIDE, sndpkt[nextseqnum].packet);
    sndpkt[nextseqnum].sent = true;

    if (base == nextseqnum) {
      // If the next packet to be sent is at the base, restart timer
      printf("- Started timer\n");
      starttimer(A_SIDE, TIMEOUT, NULL);
    }
  }
  else {
    printf("- Packet %d buffered in sending buffer.\n", nextseqnum);
  }
  nextseqnum++;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(pkt packet) {
  /* TODO: packet is possibly corrupted, check CHECKSUM?
   *       This function verifies that the packet is ACKED
   */
  printf("- A side received from Lower Layer:\t\t%s\t\t\tack# = %d\tseq# = %d\n", packet.payload,
      packet.acknum, packet.seqnum);
	if (compute_checksum(packet) != packet.checksum) {
    // Packet is corrupt - ignore
    printf("- A side received corrupt packet :(\n");
		return;
	}

	printf("- A side received valid packet.\n");
	printf("** %d MESSAGES HAVE BEEN SENT SUCCESSFULLY **\n", packet.acknum + 1);

	// If acknum != base, one or more packets in the sending window have been successfully sent.
	if (packet.acknum >= base) {
		base = packet.acknum + 1;
		printf("- Updating sending window...\n");
		if (base == nextseqnum) {
			printf("- All packets in the sending window\n"
					"  successfully received.\n");
			printf("- Stopped timer\n");
			stoptimer(A_SIDE, NULL);
		}
		else {
			printf("- Updated ACKS, restarted timer to reflect\n"
					"  the oldest packet in the sending window.\n");
			stoptimer(A_SIDE, NULL);
			starttimer(A_SIDE, TIMEOUT, NULL);
			// send all packets in the new window (if not already sent)
			printf("- A side making sure all packets in sending\n"
					"  window have been sent to B side...\n");
			for (int i = base; i < base + WINDOW; i++) {
				if (i == nextseqnum) {
					break;
				}
				else if (!sndpkt[i].sent) {
					printf("\t- Sending NEW buffered message:\t\t\t%s\tack# = %d\tseq# = %d\n",
							sndpkt[i].packet.payload, sndpkt[i].packet.acknum,
							sndpkt[i].packet.seqnum);
					tolayer3(A_SIDE, sndpkt[i].packet);
					sndpkt[i].sent = true;
				}
				else {
					printf("\t- Message in window, already sent:\t\t%s\tack# = %d\tseq# = %d\n",
							sndpkt[i].packet.payload, sndpkt[i].packet.acknum,
							sndpkt[i].packet.seqnum);
				}
			}
		}
	}

}

/* called when A's timer goes off */
void A_timerinterrupt(void *adata) {
  /* TODO: Control retransmission of packets. adata is additional data to
   * distinguish timers on the same calling entity
   */

  /* When the timer goes off, that means the oldest packet in the window failed
   * to be delivered in the specified amount of time. To make sure all packets
   * in the window arrive, send all packets in the window, starting at the
   * oldest packet (base), and restart the timer.
   */
  starttimer(A_SIDE, TIMEOUT, NULL);
  printf("- TIMEOUT - timer restarted,\n"
      "  all packets in window resent\n");
  for (int i = base; i < base + WINDOW; i++) {
		if (i == nextseqnum) {
		  break;
		}
    printf("\t- Sending Message:\t\t\t\t%s\tack# = %d\tseq# = %d\n",
        sndpkt[i].packet.payload, sndpkt[i].packet.acknum,
        sndpkt[i].packet.seqnum);
    tolayer3(A_SIDE, sndpkt[i].packet);
  }
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
  nextseqnum = 0;
  base = 0;
}

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(pkt packet) {
  /* TODO: Send ACK for packet. Verify that packet has arrived correctly and
   * intact. Send packet to upper layer.
   */
  printf("- B side received packet:\t\t\t%s\tack# = %d\tseq# = %d\n",
        packet.payload, packet.acknum, packet.seqnum);

  struct pkt ack_pkt = { 0 };

  if (compute_checksum(packet) == packet.checksum) {
    if (packet.seqnum == expected_seqnum) {
      // seqnum is what we're expecting - put in outbound buffer and send
      // packets that you can.
      printf("- B side received valid packet.\n"
          "  Sending pkt to upper layer.\n");
      tolayer5(B_SIDE, packet.payload);

      ack_pkt.seqnum = expected_seqnum;
      ack_pkt.acknum = packet.acknum;
      expected_seqnum++;
    }
    else {
      // seqnum is greater/lower than what we're expecting - discard, re-ack highest seq # pkt.
      char * type = (packet.seqnum < expected_seqnum) ? "lower" : "greater";
      printf("- B side received a packet with a %s\n"
          "  sequence number than expected. Expected\n"
          "  %d, got %d.\n"
          "  Discarding packet...\n", type, expected_seqnum, packet.seqnum);
      ack_pkt.seqnum = expected_seqnum - 1;
      ack_pkt.acknum = expected_seqnum - 1;
    }
  }
  else {
    // Packet is corrupted - checksum failed
    printf("- B side received corrupted packet.\n");
    ack_pkt.seqnum = expected_seqnum - 1;
    ack_pkt.acknum = expected_seqnum - 1;
  }

  // Send ACK to A side (containing ACK for pkt w/ highest in-order sequence number)
  memmove(ack_pkt.payload, "ACK", 3);
  ack_pkt.checksum = compute_checksum(ack_pkt);

  printf("- B side sending ACK for packet with highest\n"
      "  in-order sequence number\n");
  printf("- B side sent ACK:\t\t\t\t%s\t\t\tack# = %d\tseq# = %d\n", ack_pkt.payload,
      ack_pkt.acknum, ack_pkt.seqnum);

  tolayer3(B_SIDE, ack_pkt);
}

/* the following rouytine will be called once (only) before any other */
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

/* Creates packet to send to side B
 */
struct sending create_pkt(int seq, int ack, char *payload) {
  struct pkt new_pkt = { 0 };
  new_pkt.seqnum = seq;
  new_pkt.acknum = ack;
  memmove(new_pkt.payload, payload, MESSAGE_LEN);
  new_pkt.checksum = compute_checksum(new_pkt);

  struct sending snd = { 0 };
  snd.packet = new_pkt;
  snd.start_time = time;
  snd.sent = false;

  return snd;
}
