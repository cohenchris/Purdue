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
 * #									     																									 #
 * #											SELECTIVE-REPEAT PROTOCOL		  	     							 #
 * #									     																								   #
 * ###########################################################################
 */


/********* STUDENTS WRITE THE NEXT SIX ROUTINES *********/

#define A_SIDE 0
#define B_SIDE 1
#define WINDOW 8
#define MAXBUFF 100

int compute_checksum(pkt);

// A SIDE
typedef struct sending {
  struct pkt packet;
	bool sent;
	bool received;
} sending;
struct sending create_pkt(int, int, char *);
struct sending sndpkt[MAXBUFF];	// A-side's packets that it's sending to lower layer
int nextseqnum;									// Next packet sequence number to send
int a_base;										  // Beginning of sender buffer window

// B SIDE
typedef struct receiving {
  struct pkt packet;
	bool sent;
	bool exists;
} receiving;
struct receiving buffer_pkt(struct pkt);	// Buffers pkts at receiver
struct receiving rcvpkt[MAXBUFF];	// B-side's packets that it's sending to upper layer
int expected_seqnum;							// Next in-order seq# that receiver wants
int b_base;												// Beginning of receiver buffer window


/* called from layer 5, passed the data to be sent to other side */
void A_output(msg message) {
  /* TODO: Ensure that the message (from upper layer) is delivered
   * in-order, and correctly to the receiving side upper layer.
   *    CHECKSUM?
   */
	// TODO: DELETE LATER
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

	if (nextseqnum < a_base + WINDOW) {
	  // Next packet is within the sending window

		// Send message to lower layer
		printf("- Sending Message:\t\t\t\t%s\tack# = %d\tseq# = %d\n",
				sndpkt[nextseqnum].packet.payload, sndpkt[nextseqnum].packet.acknum,
				sndpkt[nextseqnum].packet.seqnum);
		tolayer3(A_SIDE, sndpkt[nextseqnum].packet);
		sndpkt[nextseqnum].sent = true;

		// Start timer for each packet - adata contains sequence number so we know it's received
		printf("- Started timer for packet %d\n", nextseqnum);
		starttimer(A_SIDE, TIMEOUT, &sndpkt[nextseqnum].packet);
	}
  else {
    printf("- Too many packets in-flight. Buffering to send later...\n");
  }
	nextseqnum++;
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(pkt packet) {
  /* TODO: packet is possibly corrupted, check CHECKSUM?
   *       This function verifies that the packet is ACKED
   */
	printf("- A side received from lower layer:\t\t%s\t\t\tack# = %d\tseq# = %d\n", packet.payload, packet.acknum, packet.seqnum);

	if (compute_checksum(packet) != packet.checksum) {
	  // Packet is corrupt - ignore
		printf("- A side received corrupt packet :(\n");
		return;
	}

	printf("- A side received valid packet.\n");

	if (sndpkt[packet.acknum].received) {
		// If ACK has been received before, timer has already been stopped.
	  printf("- Received duplicate ACK for packet. Ignoring...%d\n", packet.acknum - 1);
		return;
	}

	// If ACK hasn't been received before, stop the timer since it has been successfully sent.
	sndpkt[packet.acknum].received = true;
	printf("- New ACK received (packet %d) - STOPPED TIMER\n", packet.acknum);
	stoptimer(A_SIDE, &sndpkt[packet.acknum].packet);

	// COUNT NUMBER OF MESSAGES SENT
	int numsent = 0;
	while (sndpkt[numsent].received) {
		numsent++;
	}
	printf("** %d MESSAGES HAVE BEEN SENT SUCCESSFULLY **\n", numsent);

	// Update sending window - move sending window base to next lowest unACKed
	sndpkt[packet.acknum].received = true;
	printf("- Updating sending window...\n");
	a_base = numsent;

	// Make sure all packets in window have been sent
	printf("- Making sure all packets in the sending\n"
			"  window have been sent.\n");
	for (int i = a_base; i < a_base + WINDOW; i++) {
	  if (i == nextseqnum) {
			break;
		}
		else if (!sndpkt[i].sent) {
		  printf("\t- Sending NEW buffered message:\t\t\t%s\tack# = %d\tseq# = %d\n",
					sndpkt[i].packet.payload, sndpkt[i].packet.acknum,
					sndpkt[i].packet.seqnum);

			// Start timer for this packet
			printf("- Started timer for packet %d\n", sndpkt[i].packet.seqnum);
			starttimer(A_SIDE, TIMEOUT, &sndpkt[i].packet);

			// Send packet to lower layer
			tolayer3(A_SIDE, sndpkt[i].packet);
			sndpkt[i].sent = true;
		}
		else {
		  // Packet has already been sent
			printf("\t- Message in window, already sent:\t\t%s\tack# = %d\tseq# = %d\n",
					sndpkt[i].packet.payload, sndpkt[i].packet.acknum,
					sndpkt[i].packet.seqnum);
		}
	}
}

/* called when A's timer goes off */
void A_timerinterrupt(void *adata) {
  /* TODO: Control retransmission of packets. adata is additional data to
   * distinguish timers on the same calling entity
   */
	struct pkt * interrupted = (struct pkt*)adata;
	int seqnum = interrupted->seqnum;
	printf("- TIMEOUT (packet %d) - timer restarted\n"
			"  packet %d resent:\t\t\t\t%s\tack# = %d\tseq# = %d\n", seqnum, seqnum, interrupted->payload,
			interrupted->acknum, interrupted->seqnum);
	tolayer3(A_SIDE, *interrupted);
	starttimer(A_SIDE, TIMEOUT, adata);
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	nextseqnum = 0;
	a_base = 0;
}

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(pkt packet) {
  /* TODO: Send ACK for packet. Verify that packet has arrived correctly and
   * intact. Send packet to upper layer.
   */
	printf("- B side received packet:\t\t\t%s\tack# = %d\tseq# = %d\n",
			packet.payload, packet.acknum, packet.seqnum);

	struct pkt ack_pkt = { 0 };

	if (compute_checksum(packet) != packet.checksum) {
		printf("- B side received corrupt packet. Ignoring...\n");
		return;
	}
	else {
		// Packet is valid - put in buffer
		buffer_pkt(packet);

		if (packet.seqnum == expected_seqnum) {
		  // seqnum is what we're expecting - adjust base as far as it can go in the window
			printf("- In-order packet received (seq# %d)!\n"
					"  Sending all buffered packets in the window.\n", packet.seqnum);
			ack_pkt.seqnum = packet.seqnum;
			ack_pkt.acknum = packet.acknum;
			for (int i = b_base; i < b_base + WINDOW; i++) {
			  if (!rcvpkt[i].exists) {
					// If pkt doesn't exist, we need that packet next.
					b_base = i;
					expected_seqnum = i;
					printf("- Updating receiver buffer window.\n"
							"  Now expecting packet %d\n", expected_seqnum);
					break;
				}
				else {
					// Packet exists and is being sent to upper side. Stop the timer
					// for it so no repeat packets are sent.
				  printf("\t- B side sending packet to upper\n"
							"\t  layer (seq# %d).\n", rcvpkt[i].packet.seqnum);
					tolayer5(A_SIDE, rcvpkt[i].packet.payload);
				}
			}
		}
		else if ((packet.seqnum < b_base + WINDOW) &&
				     (packet.seqnum >= b_base)) {
			// Do nothing - packet is in the receiver buff window, but we don't have all packets
			// until this one. Just buffer it and wait.
			printf("- Out-of-order packet received (seq# %d).\n"
					"  Expecting seq# %d. Buffering for later...\n", packet.seqnum, expected_seqnum);
			ack_pkt.seqnum = packet.seqnum;
			ack_pkt.acknum = packet.acknum;
		}
    else if ((packet.seqnum >= b_base - WINDOW) &&
             (packet.seqnum < b_base)) {
			// Already ACKed this packet, but side A didn't receive the ACK. Resend it so side A knows
			printf("- Previously ACKed packet received. Re-ACKing...\n");
			ack_pkt.seqnum = packet.seqnum;
			ack_pkt.acknum = packet.acknum;
    }
		else {
      // Packet is too far outside the receiving window. Don't re-ACK
      printf("- Packet is too far outside the receiving window. Ignoring...\n");
      return;
		}
	}

	// Create ack_pkt and send to other side
	memmove(ack_pkt.payload, "ACK", 3);
	ack_pkt.checksum = compute_checksum(ack_pkt);

	printf("- B side sending ACK for packet %d:\t\t%s\t\t\tack# = %d\tseq# = %d\n",
			ack_pkt.seqnum, ack_pkt.payload, ack_pkt.acknum, ack_pkt.seqnum);

	tolayer3(B_SIDE, ack_pkt);
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init() {
	expected_seqnum = 0;
	b_base = 0;
	for (int i = 0; i < MAXBUFF; i++) {
	  rcvpkt[i].exists = false;
	}
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
	snd.received = false;
  snd.sent = false;

  return snd;
}

/* Buffers a received packet in side B's buffer
 */
struct receiving buffer_pkt(struct pkt packet) {
	struct receiving temp = { 0 };
	temp.packet = packet;
	temp.sent = false;
	temp.exists = true;
  rcvpkt[packet.seqnum] = temp;
}
