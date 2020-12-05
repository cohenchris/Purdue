#include <stdio.h>

#include "hw0.h"

int g_hailstone_array[ARRAY_SIZE];

/*  This function is used to run the compute_hailstone()
 *  and check_hailstone() functions implemented in file hw0.c.
 *  User input is used to determine which function to run
 *  and what input is passed to it. Upon completion, the
 *  function returns zero.
 */

int main() {
  int count = 0;
  int choice = 0;
  int initial = 0;
  int sum = 0;
  
  while (choice != 3) {
    printf("Select a function to test:\n"
           "   1) compute_hailstone()\n"
           "   2) check_hailstone()\n"
           "   3) Exit\n"
           "Your choice? ");

    fscanf(stdin, "%d", &choice);

    int limit = 0;
    int value = 0;
    switch (choice) {
      case 1:
        printf("Enter the initial value: ");
        fscanf(stdin, "%d", &initial);
        printf("Enter a limit: ");
        fscanf(stdin, "%d", &limit);
        if (limit > ARRAY_SIZE) {
          printf("You should choose a limit less than %d!\n",
                 ARRAY_SIZE + 1);
        }
        else {
          sum = compute_hailstone(initial, limit);
          printf("The resulting array looks like:\n");
          for (count = 0; count < ARRAY_SIZE; count++) {
            printf("%d\t", g_hailstone_array[count]);
            if (count % 10 == 9) {
              printf("\n");
            }
          }
          printf("The computed sum = %d\n", sum);
        }
        break;

      case 2:
        printf("Enter numbers to put into the array.\n"
               "Enter a negative number to end early.\n");
        for (count = 0; count < ARRAY_SIZE; count++) {
          printf("Element %2d: ", count);
          fscanf(stdin, "%d", &value);
          if (value < 0) {
            break;
          }
          else {
            g_hailstone_array[count] = value;
          }
        }
        printf("Enter a Limit: ");
        fscanf(stdin, "%d", &limit);

        value = check_hailstone(limit);
        printf("Value returned from Check_Hailstone(%d) = %d\n", limit, value);
        break;

      case 3:
        printf("Goodbye!\n");
        break;

      default:
        printf("Select one of the given options...!\n");
        break;
    }
  }

  return 0;
} /* main() */
