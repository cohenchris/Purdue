#include "hw8.h"

#include <stdio.h>
#include <unistd.h>

#define QUIT (0)
#define CREATE_SINGLE (1)
#define CREATE_DOUBLE (2)
#define APPEND_SINGLE (3)
#define APPEND_DOUBLE (4)
#define DELETE_SINGLE_LIST (5)
#define DELETE_DOUBLE_LIST (6)
#define SINGLE_TO_DOUBLE (7)
#define PRINT_SINGLE_LIST (8)
#define PRINT_DOUBLE_LIST (9)

/*
 *  This function prints the first name, last name, and age of the
 *  single_list argument.
 */

void print_single(single_list *single_list_ptr) {
  printf("Name: %s, %s\n"
         "age:  %d\n",
         single_list_ptr->last_name, single_list_ptr->first_name,
         single_list_ptr->age);
  return;
} /* print_single() */

/*
 *  This function prints the first name, last name, and age of the
 *  double_list argument.
 */

void print_double(double_list *double_list_ptr) {
  printf("Name: %s, %s\n"
         "age:  %d\n",
         double_list_ptr->last_name, double_list_ptr->first_name,
         double_list_ptr->age);
  return;
} /* print_double() */

/*
 *  This function prints the values for each single_list in the list
 *  passed in as an argument.
 */

void print_single_list(single_list *single_list_ptr ) {
  printf("The singly-linked list looks like:\n");
  while (single_list_ptr != NULL) {
    print_single(single_list_ptr);
    single_list_ptr = single_list_ptr->next_ptr;
  }
} /* print_single_list() */

/*
 *  This function prints the values for each double_list in the list
 *  passed in as an argument.
 */

void print_double_list(double_list *double_list_ptr ) {
  printf("The doubly-linked list looks like:\n");
  while (double_list_ptr != NULL) {
    print_double(double_list_ptr);
    double_list_ptr = double_list_ptr->next_ptr;
  }
} /* print_double_list() */

/*
 *  This function is used to run the functions in hw8.c. User input is used
 *  to determine which function to run and what input is passed to it. main()
 *  prints information related to running the chosen function.
 */

int main() {
  single_list *single_list_ptr = NULL;
  double_list *double_list_ptr = NULL;
  single_list *singly_linked_list = NULL;
  double_list *doubly_linked_list = NULL;

  // Disable stdlib I/O buffering

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) create_single(first, last, age)\n"
           "2) create_double(first, last, age)\n"
           "3) append_single()\n"
           "4) append_double()\n"
           "5) delete_single_list()\n"
           "6) delete_double_list()\n"
           "7) single_to_double\n"
           "8) Print the single list\n"
           "9) Print the double list\n"
           "\nSelect a function: ");

    int selected_num = 333333;
    scanf("%d", &selected_num);
    int age = 0;
    char first_name[100] = "\0";
    char last_name[100] = "\0";

    if ((selected_num > PRINT_DOUBLE_LIST) || (selected_num < QUIT)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (selected_num == QUIT) {
      printf("\n\n\n\n\n\nHave a nice day.\n");
      return 0;
    }
    else if (selected_num == CREATE_SINGLE) {
      if (single_list_ptr != NULL) {
        printf("WARNING: You've already allocated a single_list element.\n"
               "Deallocating it now.\n");
        delete_single_list(single_list_ptr);
      }
      printf ("Enter the first_name: ");
      scanf("%s", first_name);
      printf ("Enter the last_name:  ");
      scanf("%s", last_name);
      printf ("Enter the age:        ");
      scanf("%d", &age);
      single_list_ptr = create_single(first_name, last_name, age);
      printf("The structure looks like:\n");
      print_single(single_list_ptr);
    }
    else if (selected_num == CREATE_DOUBLE) {
      if (double_list_ptr != NULL) {
        printf("WARNING: You've already allocated a double_list element.\n"
               "Deallocating it now.\n");
        delete_double_list(double_list_ptr);
      }
      printf ("Enter the first_name: ");
      scanf("%s", first_name);
      printf ("Enter the last_name:  ");
      scanf("%s", last_name);
      printf ("Enter the age:        ");
      scanf("%d", &age);
      double_list_ptr = create_double(first_name, last_name, age);
      printf("The structure looks like:\n");
      print_double(double_list_ptr);
    }
    else if (selected_num == APPEND_SINGLE) {
      if (single_list_ptr == NULL) {
        printf("You need to use option 1 first...\n");
        sleep(3);
        continue;
      }
      if (singly_linked_list == NULL) {
        printf("NOTE: The current singly-linked list is NULL.\n"
               "Moving the single_list element to become the list.\n");
        singly_linked_list = single_list_ptr;
        single_list_ptr = NULL;
      }
      else {
        printf("Appending the single_list element"
               "after the head of the list.\n");
        append_single(singly_linked_list, single_list_ptr);
        single_list_ptr = NULL;
      }
    }
    else if (selected_num == APPEND_DOUBLE) {
      if (double_list_ptr == NULL) {
        printf("You need to use option 2 first...\n");
        sleep(3);
        continue;
      }
      if (doubly_linked_list == NULL) {
        printf("NOTE: The current doubly-linked list is NULL.\n"
               "Moving the double_list element to become the list.\n");
        doubly_linked_list = double_list_ptr;
        double_list_ptr = NULL;
      }
      else {
        printf("Appending the double_list element"
               "after the head of the list.\n");
        append_double(doubly_linked_list, double_list_ptr);
        double_list_ptr = NULL;
      }
    }

    else if (selected_num == DELETE_SINGLE_LIST) {
      printf("Deleting the singly-linked list.\n");
      delete_single_list(singly_linked_list);
      singly_linked_list = NULL;
    }
    else if (selected_num == DELETE_DOUBLE_LIST) {
      printf("Deleting the doubly-linked list.\n");
      delete_double_list(doubly_linked_list);
      doubly_linked_list = NULL;
    }

    else if (selected_num == SINGLE_TO_DOUBLE) {
      if (doubly_linked_list != NULL) {
        printf("WARNING: The doubly-linked list is not NULL.\n"
               "Deleting it first...\n");
        delete_double_list(doubly_linked_list);
        doubly_linked_list = NULL;
      }
      doubly_linked_list = single_to_double( singly_linked_list );
    }

    else if (selected_num == PRINT_SINGLE_LIST) {
      print_single_list( singly_linked_list );
    }
    else {
      print_double_list( doubly_linked_list );
    }
  }

  /* not reached. */

  return 0;
} /* main() */
