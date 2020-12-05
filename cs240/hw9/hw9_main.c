#include <stdio.h>
#include <unistd.h>

#include "hw9.h"

/*
 *  In case you're curious, we need to include unistd.h to declare
 *  the sleep() function.
 */

#define QUIT (0)
#define CREATE_DOUBLE (1)
#define APPEND_DOUBLE (2)
#define INSERT_IN_ORDER (3)
#define REMOVE_DOUBLE (4)
#define DELETE_DOUBLE (5)
#define DELETE_DOUBLE_LIST (6)
#define PRINT_DOUBLE_LIST_LIST (7)
#define REWIND_LIST (8)
#define SLEEP (3)

/*
 *  This function prints the first_name, last_name, and age of the
 *  double_list passed to it.
 */

void print_double_list(double_list *ptr) {
  printf("Name: %s, %s\tage: %d\n",
         ptr->last_name, ptr->first_name, ptr->age);
} /* print_double_list() */

/*
 *  This function prints the first_names, last_names, and ages of
 *  of each double_list in the list passed to it.
 */

void print_double_list_list(double_list *ptr) {
  printf("The doubly-linked list looks like:\n");
  while (ptr != NULL) {
    print_double_list(ptr);
    ptr = ptr ->next_ptr;
  }
} /* print_double_list_list() */

/*
 *  This function is used to run the functions in hw9.c User input is used
 *  to determine which funciton to run and what input is passed to it. main()
 *  prints information related to running the chosen function.
 */

int main() {
  double_list *d_ptr = NULL;
  double_list *d_list = NULL;

  // Disable stdlib I/O buffering

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) create_double(first, last, age)\n"
           "2) append_double()\n"
           "3) insert_in_order()\n"
           "4) remove_double()\n"
           "5) delete_double()\n"
           "6) delete_double_list()\n"
           "7) Print the double_list list.\n"
           "8) Rewind list pointer to beginning of list.\n"
           "\nSelect a function: ");

    int selected_option = 333333;
    scanf("%d", &selected_option);

    if ((selected_option > REWIND_LIST) || (selected_option < QUIT)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (selected_option == QUIT) {
      printf("\n\n\n\n\n\nHave a nice day.\n");
      return 0;
    }
    else if (selected_option == CREATE_DOUBLE) {
      if (d_ptr != NULL) {
        printf("WARNING: You've already allocated a double_list element.\n"
               "Deallocating it now.\n");
        delete_double(&d_ptr);
      }

      int age = 0;
      char last_name[100] = "\0";
      char first_name[100]  = "\0";

      printf ("Enter the first_name: ");
      scanf("%s", first_name);
      printf ("Enter the last_name:  ");
      scanf("%s", last_name);
      printf ("Enter the age:        ");
      scanf("%d", &age);
      create_double(&d_ptr, first_name, last_name, age);
      printf("The structure looks like:\n");
      print_double_list(d_ptr);
    }
    else if (selected_option == APPEND_DOUBLE) {
      if (d_ptr == NULL) {
        printf("You need to use option 1 or 4 first...\n");
        sleep(SLEEP);
        continue;
      }
      printf("Appending the double_list element after the head of the list.\n");
      append_double(&d_list, d_ptr);
      d_ptr = NULL;
    }
    else if (selected_option == INSERT_IN_ORDER) {
      if (d_ptr == NULL) {
        printf("You need to use option 1 or 4 first...\n");
        sleep(SLEEP);
        continue;
      }
      printf("Inserting the double_list element into the ordered list.\n");
      insert_in_order(&d_list, d_ptr);
      d_ptr = NULL;
    }
    else if (selected_option == REMOVE_DOUBLE) {
      if (d_ptr != NULL) {
        printf("Warning: you already have a solitary double_list structure.\n"
               "We'll delete it and the one you remove from the list will\n"
               "take its place.\n");
        delete_double(&d_ptr);
      }
      printf("Removing an element from the doubly-linked list.\n");
      d_ptr = d_list;
      remove_double(&d_list, d_ptr);
    }
    else if (selected_option == DELETE_DOUBLE) {
      if (d_ptr == NULL) {
        printf("You need to use option 1 or 4 first...\n");
        sleep(SLEEP);
        continue;
      }
      printf("Deleting the doubly-linked list.\n");
      delete_double(&d_ptr);
    }
    else if (selected_option == DELETE_DOUBLE_LIST) {
      delete_double_list(&d_list);
    }
    else if (selected_option == PRINT_DOUBLE_LIST_LIST) {
      print_double_list_list(d_list);
    }
    else {
      while (d_list != NULL && d_list->prev_ptr != NULL) {
        d_list = d_list->prev_ptr;
      }
    }
  }

  /* not reached. */

  return 0;
} /* main() */

