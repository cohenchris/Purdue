/*
 * Chris Cohen, hw8.c, CS 24000, Fall 2018
 * Last updated October 18, 2018
 */

#include "hw8.h"

#include<malloc.h>
#include<assert.h>
#include<string.h>

/* This function dynamically allocates memory for a single_list structure.
 * It allocates memory for the first_name and copies the first argument into it.
 * Then, allocates memory for last_name and fill it with a copy of the second
 * argument. Sets age field to the last argument. The next_ptr is set to NULL.
 */

single_list * create_single(char * first, char * last, int age_val) {
  assert(first != NULL);
  assert(last != NULL);

  struct single_list * new_single = malloc(sizeof(single_list));
  assert(new_single != NULL);

  /* This block allocates memory for first_name pointer in the struct based on
   * the length of first_name. It uses strlen(), then increments first_length
   * after strlen() is done, to account for the NULL character. Then, strcpy
   * is used to assign a COPY of 'first' to first_name pointer in the struct.
   */

  int first_length = strlen(first);
  first_length++;
  new_single->first_name = malloc(first_length * sizeof(char));
  assert(new_single->first_name != NULL);
  strcpy(new_single->first_name, first);

  /* This block is the same as the previous one, but for last_name pointer. It
   * allocates memory for last_name pointer based on value of strlen(last) + 1,
   * then assigns a COPY of 'last' to the last_name pointer in the struct.
   */

  int last_length = strlen(last);
  last_length++;
  new_single->last_name = malloc(last_length * sizeof(char));
  assert(new_single->last_name != NULL);
  strcpy(new_single->last_name, last);

  new_single->age = age_val;
  new_single->next_ptr = NULL;

  return new_single;
} /* create_single() */

/* This function is the same as create_single, but allocates a double_list and
 * initializes the prev_ptr to NULL.
 */

double_list * create_double(char * first, char * last, int age_val) {
  assert(first != NULL);
  assert(last != NULL);

  struct double_list * new_double = malloc(sizeof(double_list));
  assert(new_double != NULL);

  /* This block allocates memory for first_name pointer in the struct based on
   * length of the first_name. It uses strlen(), then increments first_length
   * after strlen() is done, to account for the NULL character. Then, strcpy
   * is used to assign a COPY of 'first' to first_name pointer in the struct.
   */

  int first_length = strlen(first);
  first_length++;
  new_double->first_name = malloc(first_length * sizeof(char));
  assert(new_double->first_name != NULL);
  strcpy(new_double->first_name, first);

  /* This block is the same as the previous one, but for last_name pointer. It
   * allocates memory for last_name pointer based on value of strlen(last) + 1,
   * then assigns a COPY of 'last' to the last_name pointer in the struct.
   */

  int last_length = strlen(last);
  last_length++;
  new_double->last_name = malloc(last_length * sizeof(char));
  assert(new_double->last_name != NULL);
  strcpy(new_double->last_name, last);

  new_double->age = age_val;
  new_double->next_ptr = NULL;
  new_double->prev_ptr = NULL;

  return new_double;
} /* create_double() */

/* The first argument points to a single-list structure that's an element of a
 * singly-linked list. The second argument points to a solitary single_list
 * structure whose next_ptr field is NULL. Appends the solitary elements to the
 * list element pointed to by the first argument.
 */

void append_single(single_list * single1, single_list * single2) {
  assert(single1 != NULL);
  assert(single2 != NULL);
  assert(single2->next_ptr == NULL);

  struct single_list * temp_next_ptr = single1->next_ptr;

  single1->next_ptr = single2;
  single2->next_ptr = temp_next_ptr;
} /* append_single() */

/* This function is the same as append_single, but for a doubly-linked list and
 * a solitary double_list element.
 */

void append_double(double_list * double1, double_list * double2) {
  assert(double1 != NULL);
  assert(double2 != NULL);
  assert(double2->next_ptr == NULL);

  struct double_list * temp_next_ptr = double1->next_ptr;

  double2->next_ptr = temp_next_ptr;
  double2->prev_ptr = double1;

  double1->next_ptr = double2;
  if (double2->next_ptr != NULL) {
    temp_next_ptr->prev_ptr = double2;
  }
} /* append_double() */

/* Forms a doubly-linked list with the same number of elements and contents as
 * the singly-linked list, and return a pointer to it. If the argument is NULL,
 * return NULL. The singly-linked list remains intact and unchanged.
 */

double_list * single_to_double(single_list * single) {
  if (single == NULL) {
    return NULL;
  }

  struct double_list * new_double = NULL;
  struct single_list * temp_single = single;
  struct double_list * new_head = NULL;
  while (temp_single != NULL) {
    struct double_list * next_double = create_double(temp_single->first_name,
                                                     temp_single->last_name,
                                                     temp_single->age);

    /* Checks to see if the new_double field is NULL. If it is, that
     * means that next_double is the first element in the list, so
     * new_double is equal to next_double. For simplicity, if this if
     * statement executes, the new_head pointer is assigned at the
     * same time.
     */

    if (new_double == NULL) {
      new_double = next_double;
      new_head = next_double;
    }
    else {
      append_double(new_double, next_double);
      new_double = new_double->next_ptr;
    }

    temp_single = temp_single->next_ptr;
  }

  return new_head;
} /* single_to_double() */

/* Deallocates the argument and the strings that its fields point to. Doesn't
 * deallocate the structure pointed to by next_ptr.
 */

void delete_single(single_list * list) {
  assert(list != NULL);

  free(list->first_name);
  list->first_name = NULL;
  free(list->last_name);
  list->last_name = NULL;
  free(list);
  list = NULL;
} /*delete_single ()*/

/* This function is just like delete_single except that it will deallocate a
 * double_list strucutre and the strings it points to. Doesn't deallocate
 * next_ptr or prev_ptr.
 */

void delete_double(double_list * list) {
  assert(list != NULL);

  free(list->first_name);
  list->first_name = NULL;
  free(list->last_name);
  list->last_name = NULL;
  free(list);
  list = NULL;
} /* delete_double() */

/* Deallocates each element of (argument 1). If the parameter is null, nothing
 * is done.
 */

void delete_single_list(single_list * list) {
  while (list != NULL) {
    struct single_list * temp_single = list;
    list = list->next_ptr;
    delete_single(temp_single);
  }
} /* delete_single_list() */

/* This function works the same as delete_single_list, except it deallocates
 * the elements of a double_list.
 */

void delete_double_list(double_list * list) {
  while (list != NULL) {
    struct double_list * temp_double = list;
    list = list->next_ptr;
    delete_double(temp_double);
  }
} /* delete_double_list() */


