/* ******************************************************************
Programming assignment 3: implementing distributed, asynchronous,
						  distance vector routing.

THIS IS THE HEADER FILE.  IT SHOULD NOT BE TOUCHED AT ALL BY STUDENTS!

**********************************************************************/

#ifndef _PROG3_H_
#define _PROG3_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INF 999

extern int TRACE;		/* for my debugging */
extern float clocktime;


/* a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3() */
typedef struct rtpkt {
	int sourceid;       /* id of sending router sending this pkt */
	int destid;         /* id of router to which pkt being sent
						   (must be an immediate neighbor) */
	int mincost[4];     /* min cost to node 0 ... 3 */
} rtpkt;


/* a distance_table is the dsitance table maintained by a node. 
   Entry [i,j] in node 0 is node 0's currently computed cost 
   to node j via direct neighbor i. */
typedef struct distance_table {
	int costs[4][4];
} distance_table;


/*
 * Student callable functions in prog3.c
 */
void tolayer2(rtpkt packet);

/*
 * Functions in node?.c, need to be implemented by the student
 */
void rtinit0();
void rtinit1();
void rtinit2();
void rtinit3();
void rtupdate0(rtpkt *rcvdpkt);
void rtupdate1(rtpkt *rcvdpkt);
void rtupdate2(rtpkt *rcvdpkt);
void rtupdate3(rtpkt *rcvdpkt);
#endif // !_PROG3_H_
