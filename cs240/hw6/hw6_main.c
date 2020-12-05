#include "hw6.h"

#include <stdio.h>

#define QUIT (0)
#define READ_HUMANS (1)
#define ESTABLISH_FAMILY (2)
#define PRINT_ARRAY_ENTRY (3)
#define MIN_DIFFERENCE (0)
#define MAX_DIFFERENCE (1000)

/*
 *  This function is used to run the functions in hw6.c. User input is used
 *  to determine which function to run and what input is passed to it. main()
 *  prints the return value of the selected function along with associated
 *  information.
 */

int main() {
  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) read_humans(filename)\n"
           "2) establish_family()\n"
           "3) Print an array entry\n"
           "\nSelect a function: ");

    int selected_function = 333333;
    scanf("%d", &selected_function);

    if ((selected_function > PRINT_ARRAY_ENTRY) || (selected_function < QUIT)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (selected_function == QUIT) {
      return 0;
    }
    else if (selected_function == READ_HUMANS) {
      char filename[100] = "\0";
      printf ("Enter the name of an input file: ");
      scanf("%s", filename);
      int read_humans_result = read_humans(filename);
      printf("Value returned was: %d\n", read_humans_result);

    }
    else if (selected_function == ESTABLISH_FAMILY) {
      establish_family();

      printf("Now, try checking some values with option (3) in the menu.\n");

    }
    else if (selected_function == PRINT_ARRAY_ENTRY) {
      printf("Enter array index of entry to print: ");
      int human_array_index = 0;
      scanf("%d", &human_array_index);
      if ((human_array_index < 0) || (human_array_index > g_human_count)) {
        printf("Specified number is out of bounds.\n");
      }
      else {
        printf("Name:   %s, %s\n"
          "Mother: %s, %s\n"
          "Father: %s, %s\n",
          g_human_array[human_array_index].last_name,
          g_human_array[human_array_index].first_name,
          g_human_array[human_array_index].mother_last,
          g_human_array[human_array_index].mother_first,
          g_human_array[human_array_index].father_last,
          g_human_array[human_array_index].father_first);

        int difference = 0;
        if (g_human_array[human_array_index].mother_ptr != NULL) {
          difference = g_human_array[human_array_index].mother_ptr -
                       g_human_array;
          if ((difference > MAX_DIFFERENCE) || (difference < MIN_DIFFERENCE)) {
            printf("Something is very wrong with this entry's mother_ptr.\n");
          }
          else {
            printf("The mother_ptr points to g_human_array[%d] (%s,%s)\n",
              difference,
              (*g_human_array[human_array_index].mother_ptr).last_name,
              g_human_array[human_array_index].mother_ptr->first_name);
          }
        }

        if (g_human_array[human_array_index].father_ptr != NULL) {
          difference = g_human_array[human_array_index].father_ptr -
                       g_human_array;
          if ((difference > MAX_DIFFERENCE) || (difference < MIN_DIFFERENCE)) {
            printf("Something is very wrong with this entry's father_ptr.\n");
          }
          else {
            printf("The father_ptr points to g_human_array[%d] (%s,%s)\n",
              difference,
              g_human_array[human_array_index].father_ptr->last_name,
              g_human_array[human_array_index].father_ptr->first_name);
          }
        }

        if (g_human_array[human_array_index].child_ptr != NULL) {
          difference = g_human_array[human_array_index].child_ptr -
                       g_human_array;
          if ((difference > MAX_DIFFERENCE) || (difference < MIN_DIFFERENCE)) {
            printf("Something is very wrong with this entry's child_ptr.\n");
          }
          else {
            printf("The child_ptr points to g_human_array[%d] (%s,%s)\n",
              difference,
              g_human_array[human_array_index].child_ptr->last_name,
              g_human_array[human_array_index].child_ptr->first_name);
          }
        }

        if (g_human_array[human_array_index].sibling_ptr != NULL) {
          difference = g_human_array[human_array_index].sibling_ptr -
                       g_human_array;
          if ((difference > MAX_DIFFERENCE) || (difference < MIN_DIFFERENCE)) {
            printf("Something is very wrong with this entry's sibling_ptr.\n");
          }
          else {
            printf("The sibling_ptr points to g_human_array[%d] (%s,%s)\n",
              difference,
              g_human_array[human_array_index].sibling_ptr->last_name,
              g_human_array[human_array_index].sibling_ptr->first_name);
          }
        }
      }
    }
  }

  /* not reached. */

  return 0;
} /* main() */

