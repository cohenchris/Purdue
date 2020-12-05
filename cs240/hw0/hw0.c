/* Name, hw0.c, CS 24000, Fall 2018
 * Last updated August 16, 2018 
 */

/* Add any includes here */

#include "hw0.h"

/* This function computes a hailstone sequence for the given parameters.
 * The start parameter is the value that will be used first in the hailstone sequence,
 * and the stop parameter is the value that denotes how many times to execute the sequence. */

int compute_hailstone(int start, int stop){
  if ((start <= 0) || (stop <= 0))
    return HAILSTONE_ERROR;
  else {
    g_hailstone_array[0] = start;

    /*This for loop creates a hailstone sequence based on the 'start' value passed in through the parameters.
     * The amount of hailstones created is based on the 'stop' value that is passed in through the parameters.*/
    
    for (int i = 1; i < stop; i++){
      if (g_hailstone_array[i - 1] % 2 != 0)
        g_hailstone_array[i] = g_hailstone_array[i - 1] * 3 + 1;
      else
        g_hailstone_array[i] = g_hailstone_array[i - 1] / 2;
    }	

    /*This block of code sums up all of the hailstone values in the array and returns that value.*/

  int sum = 0;
  for (int i = 0; i < stop; i++)
    sum += g_hailstone_array[i];
  return sum;
  }
}/*compute_hailstone*/

/* This function verifies that a valid hailstone sequence, up to limit-1 (where limit is the parameter) is present in g_hailstone_array. */

int check_hailstone(int limit){
  if ((limit < 1) || (g_hailstone_array[0] < 1))
    return HAILSTONE_ERROR;
  else{
    int local_array[limit];
    int start = g_hailstone_array[0];
    local_array[0] = start;

    /* This for loop creates a hailstone sequence where the first number is the first element in the g_hailstone_array*/

    for (int i = 1; i < limit; i++){
      if (local_array[i - 1] % 2 != 0)
        local_array[i] = local_array[i - 1] * 3 + 1;
      else
        local_array[i] = local_array[i - 1] / 2;
    }

    /*This for loop iterates through both arrays and compares them. If any element from one does not exactly match up with the other,
     * the index where the arrays first differ is returned.*/
    
  for (int i = 0; i < limit; i++)
    if (local_array[i] != g_hailstone_array[i])
  return i;
  }
  return HAILSTONE_CORRECT;
}/*check_hailstone*/

/* Remember, you don't need a main function!
 * it is provided by hw0_main.c or hw0_test.o
 */

