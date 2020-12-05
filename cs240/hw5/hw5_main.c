#include "hw5.h"

#include <stdio.h>

/*
 *  This function is used to run the functions in hw5.c.
 *  User input is used to determine which function to run
 *  and what user input is passed to it. main() prints
 *  the return value of the selected function.
 */

int main() {
  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) read_humans(Filename)\n"
           "2) find_children(Last,First,Array)\n"
           "3) Print an array entry\n"
           "\nSelect a function: ");

    int selection_num = 333333;
    scanf("%d", &selection_num);

    if ((selection_num > 3) || (selection_num < 0)) {
      printf("Invalid selection.\n");
      continue;
    }

    int int_result = 0;
    int count = 0;
    if (selection_num == 0) {
      return 0;
    }
    else if (selection_num == 1) {
      char input_filename[100] = "\0";
      printf ("Enter the name of an input file: ");
      scanf("%s", input_filename);
      int_result = read_humans(input_filename);
      printf("Value returned was: %d\n", int_result);

    }
    else if (selection_num == 2) {
      char last_name[100] = "\0";
      char first_name[100] = "\0";
      int child_array[10] = {0};
      printf("Enter last name of parent:  ");
      scanf("%s", last_name);
      printf("Enter first name of parent:  ");
      scanf("%s", first_name);
      int_result = find_children(last_name, first_name, child_array);

      printf("%d children were found:\n", int_result);
      for (count = 0; count < int_result; count++ ) {
        if (child_array[count] < 0) {
          printf("\t%3d: <huh?>\n", child_array[count]);
        }
        else if (child_array[count] > g_human_count) {
          printf("\t%3d: <huh?>\n", child_array[count]);
        }
        else {
          printf("\t%3d: %s, %s\n", child_array[count],
            g_human_array[child_array[count]].last_name,
            g_human_array[child_array[count]].first_name);
        }
      }
    }
    else if (selection_num == 3) {
      printf("Enter array index of entry to print: ");
      scanf("%d", &count);
      if ((count < 0) || (count > g_human_count)) {
        printf("Specified number is out of bounds.\n");
      }
      else {
        printf("Name:   %s, %s\n"
               "Mother: %s, %s\n"
               "Father: %s, %s\n",
               g_human_array[count].last_name,
               g_human_array[count].first_name,
               g_human_array[count].mother_last,
               g_human_array[count].mother_first,
               g_human_array[count].father_last,
               g_human_array[count].father_first);
      }
    }
  }

  /* not reached. */

  return 0;
} /* main() */

