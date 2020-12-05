#include <stdbool.h>
#include "prog3.h"

#define CURR_NODE 1

distance_table dt1;
bool neighbor1[4];
void printdt1(distance_table *);

/* students to write the following two routines, and maybe some others */

/*
 * Initializes distance vector table based on the graph given in the lab handout.
 * If two nodes are not directly connected, we initialize the value to INFINITY (INF).
 * Then send distance table for current node to all adjacent nodes.
 */
void rtinit1() {
  printf("rtinit1 called at %f\n", clocktime);
  // Initialize the distance table
  dt1.costs[0][0] = INF;   // Cost from node 0 to node 0
  dt1.costs[0][1] = INF;   // Cost from node 0 to node 1
  dt1.costs[0][2] = INF;   // Cost from node 0 to node 2
  dt1.costs[0][3] = INF;   // Cost from node 0 to node 3

  dt1.costs[CURR_NODE][0] = 1;     // Cost from node 1 to node 0
  dt1.costs[CURR_NODE][1] = 0;     // Cost from node 1 to node 1
  dt1.costs[CURR_NODE][2] = 1;     // Cost from node 1 to node 2
  dt1.costs[CURR_NODE][3] = INF;   // Cost from node 1 to node 3

  dt1.costs[2][0] = INF;   // Cost from node 2 to node 0
  dt1.costs[2][1] = INF;   // Cost from node 2 to node 1
  dt1.costs[2][2] = INF;   // Cost from node 2 to node 2
  dt1.costs[2][3] = INF;   // Cost from node 2 to node 3

  dt1.costs[3][0] = INF;   // Cost from node 3 to node 0
  dt1.costs[3][1] = INF;   // Cost from node 3 to node 1
  dt1.costs[3][2] = INF;   // Cost from node 3 to node 2
  dt1.costs[3][3] = INF;   // Cost from node 3 to node 3


  // Initialize neighbor data structure - tells node if it's neighbors with a given node
  neighbor1[0] = true;
  neighbor1[1] = true;
  neighbor1[2] = true;
  neighbor1[3] = false;
  
  // Send directly-connected neighbors cost of min cost paths to all other nodes
  // ( tolayer2() )
  for (int i = 0; i < 4; i++) {
    if ((i != CURR_NODE) && (neighbor1[i])) {
      printf("Node %d sending INITIAL cost from node %d to node %d --> %d\n", CURR_NODE, CURR_NODE, i, dt1.costs[CURR_NODE][i]);
      struct rtpkt neighbor_pkt = { 0 };
      neighbor_pkt.sourceid = CURR_NODE;
      neighbor_pkt.destid = i;
      neighbor_pkt.mincost[0] = dt1.costs[CURR_NODE][0];
      neighbor_pkt.mincost[1] = dt1.costs[CURR_NODE][1];
      neighbor_pkt.mincost[2] = dt1.costs[CURR_NODE][2];
      neighbor_pkt.mincost[3] = dt1.costs[CURR_NODE][3];

      tolayer2(neighbor_pkt);
    }
  }
}

/*
 * Given a packet with new distance table information, the distance vector formula is computed
 * to  try and find any new minimum costs from the current node to any of the others.
 */
void rtupdate1(rtpkt *rcvdpkt) {
  printf("rtupdate1 received a packet that was sent from node %d at %f\n", rcvdpkt->sourceid, clocktime);
  bool updated = false;
  bool new_min = false;
  int min = INF;

  /////////////////////////////// UPDATE ///////////////////////////////////////
  // Update distance table based on packet received
  printf("----- NEW INFORMATION -----\n");
  for (int i = 0; i < 4; i++) {
    //if (rcvdpkt->mincost[i] > dt1.costs[rcvdpkt->sourceid][i]) {
      printf("Received new distance from node %d to node %d --> %d\n", rcvdpkt->sourceid, i, rcvdpkt->mincost[i]);
      dt1.costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
    //}
  }

  /////////////////////////////// COMPUTE //////////////////////////////////////
  // CALCULATE DISTANCE VECTOR ALGORITHM
  // Distance from CURR_NODE to y = min{ <cost from CURR_NODE to v> + <distance from v to y> }
  // ** taken over all nodes **
  for (int y = 0; y < 4; y++) {
    min = dt1.costs[CURR_NODE][y];
    new_min = false;
    // Find the min over all neighbors 'n' of CURR_NODE
    for (int n = 0; n < 4; n++) {
      if ((dt1.costs[CURR_NODE][n] + dt1.costs[n][y]) < min) {
        min = dt1.costs[CURR_NODE][n] + dt1.costs[n][y];
        updated = true;
        new_min = true;
      }
    }
    if (new_min) {
      // If a new minimum was found, update that entry in table
      dt1.costs[CURR_NODE][y] = min;
      printf("Calculated new minimum for path between node %d and node %d --> %d\n", CURR_NODE, y, min);
    }
  }
  if (!updated) {
    printf("** Distance table not updated **\n");
  }
  printf("---------------------------\n");

  //////////////////////////////// NOTIFY //////////////////////////////////////
  // If anything was updated, then send adjacent nodes the updated distance vector entries
  for (int i = 0; i < 4; i++) {
    if ((i != CURR_NODE) && (neighbor1[i]) && (updated)) {
      struct rtpkt neighbor_pkt = { 0 };
      neighbor_pkt.sourceid = CURR_NODE;
      neighbor_pkt.destid = i;
      neighbor_pkt.mincost[0] = dt1.costs[CURR_NODE][0];
      neighbor_pkt.mincost[1] = dt1.costs[CURR_NODE][1];
      neighbor_pkt.mincost[2] = dt1.costs[CURR_NODE][2];
      neighbor_pkt.mincost[3] = dt1.costs[CURR_NODE][3];

      printf("Sending new information from node %d to node %d\n", CURR_NODE, i);
      tolayer2(neighbor_pkt);
    }
  }

  printdt1(&dt1);
}


void printdt1(distance_table *dtptr) {
	printf("            cost to        \n");
	printf("   D1 |   0     1     2    3 \n");
	printf("  ----|----------------------\n");
	printf("     0|  %3d   %3d   %3d  %3d\n", dtptr->costs[0][0],
		dtptr->costs[0][1], dtptr->costs[0][2], dtptr->costs[0][3]);

	printf("     1|  %3d   %3d   %3d  %3d\n", dtptr->costs[1][0],
		dtptr->costs[1][1], dtptr->costs[1][2], dtptr->costs[1][3]);

	printf("from 2|  %3d   %3d   %3d  %3d\n", dtptr->costs[2][0],
		dtptr->costs[2][1], dtptr->costs[2][2], dtptr->costs[2][3]);

	printf("     3|  %3d   %3d   %3d  %3d\n", dtptr->costs[3][0],
		dtptr->costs[3][1], dtptr->costs[3][2], dtptr->costs[3][3]);
}



