/*
 * Chris Cohen, hw7.c, CS 24000, Fall 2018
 * Last updated October 11, 2018
 */

#include "hw7.h"

#include <stdlib.h>
#include <assert.h>
#include <malloc_debug.h>

/* This function will act based on the operation (argument 2). If the operation
 * is DUPLICATE, the passed string (argument 1) will be made into a linked
 * list. If the operation is REVERSE, the same thing is done, except in reverse
 * order. Once the list is created, the linked list is traversed and made
 * back into a string. Once that is done, the linked list is destroyed and a
 * pointer to the new string is returned.
 */

char * copy_string(char * str, int operation) {
  assert(str != NULL);
  assert((operation == REVERSE) || (operation == DUPLICATE));

  /* If the operation (argument 2) is 'REVERSE', the string will be transformed
   * to a linked list in reverse. If operation is not 'REVERSE', it must be
   * 'DUPLICATE', since we asserted that at the beginning.
   */

  int reverse = 0;
  if (operation == REVERSE) {
    reverse = YES;
  }
  else {
    reverse = NO;
  }

  /* A list is created using the string given (argument 1), where the 'reverse'
   * argument is determined from the above code section. The string is turned
   * into a list using string_to_list. Afterwards, a new string is created
   * by using the list just made. The list is turned into a string using
   * list_to_string. Finally, the list just created is freed using free_list,
   * and the newly created string is returned.
   */

  struct node * transformed_string = string_to_list(str, reverse);
  char * new_string = list_to_string(transformed_string);

  free_list(transformed_string);
  return new_string;

} /* copy_string() */

/* This function accepts a pointer to the string to be put in a list format
 * (argument 1), and an int reverse (argument 2) which specifies whether
 * the string will be put in the list in reverse order or not. Returns a
 * pointer to the head of the created list.
 */

struct node * string_to_list(char * str, int reverse) {
  assert(str != NULL);
  assert((reverse == YES) || (reverse == NO));

  /* The string length is not based off of the string.h function 'strlen()'
   * but instead relies on a while loop that increments the length as long
   * as the current character isn't the NULL character. Once it hits the
   * NULL character, it asserts that the current string length is greater
   * than 0, since we don't want an empty string. After, it increments
   * str_length again to account for the NULL character.
   */

  int str_length = 0;
  while(str[str_length] != '\0') {
    str_length++;
  }
  assert(str_length > 0);
  str_length++;

  /* A new list is created to hold each character of str in seperate nodes.
   * If reverse is NO, each character is appended to the new_list using
   * the function append_char.
   */

  struct node * new_list = NULL;
  if (reverse == NO) {
    int index = 0;
    while (index < str_length - 1) {
      new_list = append_char(new_list, str[index]);
      index++;
    }
  }

  /* If reverse (argument 2) isn't 'NO', it has to be 'YES', since we asserted
   * that it was either 'YES' or 'NO' at the beginning of the function. Each
   * character is prepended to the new_list using prepend_char. Then,
   * the newly created list is returned.
   */

  else {
    int index = 0;
    while (index < str_length - 1) {
      new_list = prepend_char(new_list, str[index]);
      index++;
    }
  }

  return new_list;
} /* string_to_list() */

/* This function creates a node holding the passed char (argument 2).
 * It is appended to the tail of the list and returns a pointer to the head
 * of the list. A pointer to the struct node (argument 1) is passed as well.
 * If the first argument is NULL, it is appending to a new (empty) list.
 */

struct node * append_char(struct node * list, char append) {
  assert(append >= 32);

  /* Memory is allocated for the append_char_node based on the size of
   * a single node struct (using malloc()). We also need to make sure
   * that the malloc() call did not return NULL.
   */

  struct node * append_char_node = malloc(sizeof(struct node));
  assert(append_char_node != NULL);

  append_char_node->val = append;
  append_char_node->next = NULL;

  if (list == NULL) {
    list = append_char_node;
  }

  else {
    struct node * temp_list = list;

    while (temp_list->next != NULL) {
      temp_list = temp_list->next;
    }

    temp_list->next = append_char_node;
  }

  return list;
} /* append_char() */

/* This function  creates a node holding the passed char (argument 2).
 * It is prepended to the head of the list, and a pointer to the new head of
 * the list is returned. A pointer to a struct node (argument 1) is passed as
 * well. If the first argument is null, it is prepending to a new list.
 */

struct node * prepend_char(struct node * list, char prepend) {
  assert(prepend >= 32);

  struct node * prepend_char_node = malloc(sizeof(struct node));
  assert(prepend_char_node != NULL);
  prepend_char_node->val = prepend;
  prepend_char_node->next = NULL;

  if (list != NULL) {
    prepend_char_node->next = list;
  }

  return prepend_char_node;
} /* prepend_char() */

/* This function allocates new memory for a character string and traverses
 * the list to get the characters and copy them to the newly allocated
 * string. A pointer to a struct node (argument 1) is accepted.
 */

char * list_to_string(struct node * list) {
  assert(list != NULL);

  /* A temporary list is created so that the value of 'list' can be used
   * later. The loop goes through the list and increases list_count if
   * the current node isn't NULL. After it hits a NULL node, list_count
   * is incremented again to account for the NULL character. Then,
   * memory is allocated for the new_string based on how many characters
   * there are (list_count) in the string, and the sizeof(char). We also
   * need to make sure that malloc() didn't return a NULL value.
   */

  struct node * temp_list = list;

  int list_count = 0;
  while (temp_list != NULL) {
    temp_list = temp_list->next;
    list_count++;
  }
  list_count++;

  temp_list = list;

  char * new_string = malloc(list_count * sizeof(char));
  assert(new_string != NULL);

  /* This chunk of code iterates through the linked list by making sure the
   * current node is not NULL. If it isn't it appends the character 'val'
   * from the node, then makes the 'list' variable point to the next node
   * in the linked list. After every character is added, the string is
   * NULL terminated.
   */

  int string_index = 0;
  while (temp_list != NULL) {
    new_string[string_index] = temp_list->val;
    string_index++;
    temp_list = temp_list->next;
  }
  new_string[string_index] = '\0';

  free_list(temp_list);
  return new_string;
} /* list_to_string() */

/* This function iterates through the 'list' and frees each node using the
 * free() function.
 */

void free_list(struct node * list) {
  struct node * temp_list = list;

  while (list != NULL) {
    temp_list = list;
    list = list->next;
    free(temp_list);
  }
} /* free_list() */

/* This function converts both strings passed in (arguments 1 and 2) into 2
 * lists, joins the list into one, and copies the elements of the new big
 * list into a new string. Then, frees the lists used and returns a pointer
 * to the new string.
 */

char * concat_strings(char * str1, char * str2) {
  assert(str1 != NULL);

  struct node * list1 = string_to_list(str1, NO);
  struct node * list2 = NULL;

  /* The small while loop makes sure that the length of str2 isn't 0.
   * If it is, a node named list2 is created using the elements of str2.
   * and the function string_to_list(). If it's of length 0, list2 is
   * set to NULL.
   */

  int str2_length = 0;
  while (str2[str2_length] != '\0') {
    str2_length++;
  }

  if (str2_length > 0) {
    list2 = string_to_list(str2, NO);
  }

  else {
    list2 = NULL;
  }


  append_lists(list1, list2);
  char * final_string = list_to_string(list1);

  free_list(list1);

  return final_string;
} /* concat_strings() */

/* This function makes the tail of the first list (argument 1) point to the
 * head of the second list (argument 2).
 */

void append_lists(struct node * list1, struct node * list2) {
  assert(list1 != NULL);

  /* This while loop checks to see if the next node is NULL. If not, the tail
   * has not been reached. As soon as the next node is NULL, the tail has been
   * reached.
   */

  while (list1->next != NULL) {
    list1 = list1->next;
  }

  list1->next = list2;
} /* append_lists() */
