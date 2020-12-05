#ifndef _PROG2_H_
#define _PROG2_H_

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

#define TRUE 1
#define FALSE 0
#define A 0
#define B 1

// set to 0, we only consider unidirectional cases here (from A to B)
#define BIDIRECTIONAL 0
// the amount of time that will pass before the timer interrupts
#define TIMEOUT 20
// used to limit the number of unacknowledged packets
// note that some of the packets in the window will already receive the ACKs in SR algorithm
#define WINDOWSIZE 8
// used to limit the number of packets in the buffer
// further packets will be dropped 
#define BUFFERSIZE 50

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
#define  MESSAGE_LEN  20
typedef struct msg {
	char data[MESSAGE_LEN];
} msg;

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
typedef struct pkt {
	int seqnum;
	int acknum;
	int checksum;
	char payload[MESSAGE_LEN];
} pkt;


/*
 * Functions in tasks.c
 */

void A_output(msg message);				// called from layer 5, passed the data to be sent to other side 
void A_input(pkt packet);				// called from layer 3, when a packet arrives for layer 4
void A_timerinterrupt(void *adata);		// called when A's timer goes off 
void A_init();							// called once (only) to do any initialization 
void B_input(pkt packet);				// called from layer 3, when a packet arrives for layer 4
void B_init();							// called once (only) to do any initialization 


/*
 * Student callable functions in prog2.c
 */
void stoptimer(int AorB, void *adata);
void starttimer(int AorB, float increment, void *adata);
void tolayer3(int AorB, pkt packet);
void tolayer5(int AorB, char *datasent);


#endif // _PROG2_H_
