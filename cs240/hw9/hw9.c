/*
 * Chris Cohen, hw9.c, CS 24000, Fall 2018
 * Last updated November 2, 2018
 */

#include "hw9.h"

#include<malloc.h>
#include<assert.h>
#include<string.h>

/* This function dynamically allocates memory for a double_list structure.
 * It allocates memory for the first_name, then copies argument 2 into it.
 * It then allocates memory for the last_name, then copies argument 3 into
 * it. Lastly, it sets the age field to argument 4, and the next_ptr and
 * prev_ptr to NULL. Argument 1 points to the new double_list struct created.
 */

void create_double(double_list ** list, char * first, char * last,
                   int age_val) {
  assert(list != NULL);
  assert(first != NULL);
  assert(last != NULL);

  struct double_list * new_double = malloc(sizeof(double_list));
  assert(new_double != NULL);

  /* This block allocates memory for first_name pointer in the struct based
   * on the length of the first_name. It uses strlen(), then increments
   * first_length after strlen() is done, to account for the NULL character.
   * Then, strcpy() is used to assign a COPY of 'first' to first_name pointer
   * in the struct.
   */

  int first_length = strlen(first);
  first_length++;
  new_double->first_name = malloc(first_length * sizeof(char));
  assert(new_double->first_name != NULL);
  strcpy(new_double->first_name, first);

  /* This block is the same as the previous one, but for last_name pointer.
   * It allocates memory for the last_name pointer based on value of
   * strlen(last) + 1, then assigns a COPY of 'last' to the last_name
   * pointer in the struct.
   */

  int last_length = strlen(last);
  last_length++;
  new_double->last_name = malloc(last_length * sizeof(char));
  assert(new_double->last_name != NULL);
  strcpy(new_double->last_name, last);

  new_double->age = age_val;
  new_double->next_ptr = NULL;
  new_double->prev_ptr = NULL;

  *list = new_double;
} /* create_double() */

/* Argument 1 is a double_list structure that is an element of a doubly linked
 * list. Argument 2 is a single double_list structure whose prev_ptr & next_ptr
 * values are NULL. This function appends argument 2 to the list pointed to by
 * argument 1. At the end, argument 1 is set equal to argument 2.
 */

void append_double(double_list ** list1, double_list * list2) {
  assert(list1 != NULL);
  assert(list2 != NULL);
  assert(list2->prev_ptr == NULL);
  assert(list2->next_ptr == NULL);

  /* Checks to see if the pointer that argument 1 points to is NULL, which
   * would mean we are appending to an empty list. That means that argument 2
   * will be the only element.
   */

  if ((*list1) == NULL) {
    (*list1) = list2;
    return;
  }

  else {
    /* If the pointer that argument 1 points to is the tail of a list, then
     * the next_ptr is NULL. This if-statement changes up the logic a bit
     * in order to not run into any seg faults.
     */

    if ((*list1)->next_ptr == NULL) {
      (*list1)->next_ptr = list2;
      list2->prev_ptr = (*list1);
      list2->next_ptr = NULL;
    }

    else {
      list2->next_ptr = (*list1)->next_ptr;
      list2->prev_ptr = (*list1);
      ((*list1)->next_ptr)->prev_ptr = list2;
      (*list1)->next_ptr = list2;
    }

    (*list1) = list2;
  }
} /* append_double() */

/* This function traverses the list and puts argument 2 into the list in
 * sorted order (list is already sorted). The list is ordered in ascending
 * alphabetical order by last_name, with the first name used to break ties.
 * Argument 1 is updated to point to the newly inserted element.
 */

void insert_in_order(double_list ** list1, double_list * list2) {
  assert(list1 != NULL);
  assert(list2 != NULL);
  assert(list2->prev_ptr == NULL);
  assert(list2->next_ptr == NULL);

  if (*list1 == NULL) {
    append_double(list1, list2);
    return;
  }

  /* This short while loop makes sure that the node that argument 1 points
   * to points to the tail of the doubly-linked list. This is to allow
   * append_char() to be used in this function more easily.
   */

  while ((*list1)->next_ptr != NULL) {
    (*list1) = (*list1)->next_ptr;
  }

  while ((*list1) != NULL) {
    char * last_name_insert = list2->last_name;
    char * first_name_insert = list2->first_name;

    char * current_last = (*list1)->last_name;
    char * current_first = (*list1)->first_name;

    /* This if statement checks to see if the last names are equal. If they
     * are, the tie is broken based on the first name.
     */

    if (strcmp(last_name_insert, current_last) == 0) {
      while ((*list1) != NULL) {
        first_name_insert = list2->first_name;
        last_name_insert = list2->last_name;

        current_first = (*list1)->first_name;
        current_last = (*list1)->last_name;

        /* This if statement checks to see if the last names are no longer
         * equal. If they aren't that means the name to be added is the
         * first name, alphabetically, in the group of people with the
         * same last name.
         */

        if (strcmp(last_name_insert, current_last) != 0) {
          append_double(list1, list2);
          (*list1) = list2;
          return;
        }

        /* This if statement checks to see if the first names are equal
         * too. If they are, the same name has been passed twice, so
         * it is simply appended.
         */

        if (strcmp(first_name_insert, current_first) == 0) {
          append_double(list1, list2);
          (*list1) = list2;
          return;
        }

        /* This if statement checks to see if the prev_ptr of the current
         * node is NULL. If it is, that means we're at the head of the
         * list and the name to be inserted must be either prepended or
         * appended to the current one.
         */

        if ((*list1)->prev_ptr == NULL) {
          if (strcmp(first_name_insert, current_first) > 0) {
            append_double(list1, list2);
            (*list1) = list2;
            return;
          }
          else {
            (*list1)->prev_ptr = list2;
            list2->prev_ptr = NULL;
            list2->next_ptr = (*list1);
            (*list1) = list2;
            return;
          }
        }

        /* This if statement checks to see if the person to be inserted
         * should be appended to the current node.
         */

        if (strcmp(first_name_insert, current_first) > 0) {
          append_double(list1, list2);
          (*list1) = list2;
          return;
        }

        else {
          (*list1) = (*list1)->prev_ptr;
        }
      }
    }

    else if (strcmp(last_name_insert, current_last) > 0) {
      /* This else-if statement sees if the last name to be added is
       * 'less than' the last name being pointed to by *list1. If
       * it is, that means the last name to be added directly
       * follows the current last name being looked at
       * (based on how strcmp() works).
       */

      append_double(list1, list2);
      (*list1) = list2;
      return;
    }

    /* This if statement checks to see if *list1 points to the head of the
     * list. If it does, and it reaches this point, that means that list2
     * belongs at the head of the list.
     */

    if ((*list1)->prev_ptr == NULL) {
      (*list1)->prev_ptr = list2;
      list2->prev_ptr = NULL;
      list2->next_ptr = (*list1);
      (*list1) = list2;
      return;
    }

    (*list1) = (*list1)->prev_ptr;
  }
} /* insert_in_order() */

/* This function removes argument 2 from the list of argument 1. Then, the
 * pointer that argument 1 points to should be set to point to the structure
 * immediately before the structure that was removed. If there is no
 * structure preceding the one to be removed, then let it point to the
 * structure that immediately follows the structure to be removed.
 * If the removed structure is the only element, the pointer that the first
 * argument points to should be set to NULL.
 */

void remove_double(double_list ** list1, double_list * list2) {
  assert(list1 != NULL);
  assert(list2 != NULL);

  if ((*list1) == NULL) {
    list2->next_ptr = NULL;
    list2->prev_ptr = NULL;
    return;
  }

  /* This short while loop iterates through the list backwards to make
   * sure that *list1 points to the head of the list.
   */

  while((*list1)->prev_ptr != NULL) {
    (*list1) = (*list1)->prev_ptr;
  }

  /* This while loop iterates through the next_ptr of each struct in order
   * to get to the element that is equal to list2. If *list1 is NULL,
   * that means that the end of the list has been reached and list2
   * has not been found in the list, so it sets the prev_ptr and next_ptr
   * for list2 equal to null, then stops the function.
   */

  while ((*list1) != list2) {
    if ((*list1) == NULL) {
      list2->next_ptr = NULL;
      list2->prev_ptr = NULL;
      return;
    }
    (*list1) = (*list1)->next_ptr;
  }

  /* This if statement checks to see if the current node is the only one
   * in the list. If it is, *list1 is set to NULL, and the list2 next and
   * prev ptrs are set to NULL since it was removed.
   */

  if (((*list1)->prev_ptr == NULL) &&
      ((*list1)->next_ptr == NULL)) {
    (*list1) = NULL;
    list2->prev_ptr = NULL;
    list2->next_ptr = NULL;
    return;
  }

  /* This if statement checks to see if the prev_ptr of the current *list1
   * struct is NULL. If it is, that means that it is the head of the list,
   * so *list1 is set to the next_ptr. The next_ptr's prev_ptr is set to
   * NULL since it is the new head of the list.
   */

  if ((*list1)->prev_ptr == NULL) {
    (*list1) = (*list1)->next_ptr;

    ((*list1)->prev_ptr)->next_ptr = NULL;
    ((*list1)->prev_ptr)->prev_ptr = NULL;
    list2->next_ptr = NULL;
    list2->prev_ptr = NULL;
    (*list1)->prev_ptr = NULL;
  }

  else if ((*list1)->next_ptr == NULL) {
  /* This else-if checks to see if the next_ptr of the current *list1
   * struct is NULL. If it is, that means that it is the tail of the list,
   * so *list1 is set to the prev_ptr. The prev_ptr's next_ptr is set to
   * NULL since it is the new tail of the list.
   */

    (*list1) = (*list1)->prev_ptr;

    ((*list1)->next_ptr)->next_ptr = NULL;
    ((*list1)->next_ptr)->prev_ptr = NULL;
    list2->next_ptr = NULL;
    list2->prev_ptr = NULL;
    (*list1)->next_ptr = NULL;
  }

  else {
    /* If this else statement is executed, that means that list2 is neither
     * a head nor a tail. The next_ptr's prev_ptr is set to the *list1's
     * prev_ptr. The prev_ptr's next_ptr is set to the *list1's next_ptr.
     * Then, *list1 is set to the previous pointer.
     */

    (*list1) = (*list1)->prev_ptr;

    (((*list1)->next_ptr)->next_ptr)->prev_ptr = (*list1);
    (*list1)->next_ptr = ((*list1)->next_ptr)->next_ptr;

    list2->next_ptr = NULL;
    list2->prev_ptr = NULL;
  }

} /* remove_double() */

/* This function deallocates the argument and the strings that its fields
 * point to. Sets the pointer that points to the double_list structure
 * to NULL.
 */

void delete_double(double_list ** list) {
  assert(list != NULL);
  assert(*list != NULL);
  assert((*list)->prev_ptr == NULL);
  assert((*list)->next_ptr == NULL);

  free((*list)->first_name);
  (*list)->first_name = NULL;

  free((*list)->last_name);
  (*list)->last_name = NULL;

  free(*list);
  (*list) = NULL;
} /* delete_double() */

/* This function deallocates each element of the double-linked list given
 * in the argument.
 */

void delete_double_list(double_list ** list) {

  if ((list == NULL) ||
      (*list == NULL)) {
    return;
  }

  /* This short while loop makes sure that *list1 points to the head of the
   * list by traversing it backwards and checking the prev_ptr.
   */

  while ((*list)->prev_ptr != NULL) {
    (*list) = (*list)->prev_ptr;
  }

  while (*list != NULL) {
    double_list * temp_next = *list;
    (*list) = (*list)->next_ptr;
    (temp_next)->prev_ptr = NULL;
    (temp_next)->next_ptr = NULL;
    delete_double(&temp_next);
  }

} /* delete_double_list() */
