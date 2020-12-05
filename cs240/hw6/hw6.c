/*
 * Chris Cohen, hw6.c, CS 24000, Fall 2018
 * Last updated October 14, 2018
 */

#include "hw6.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

human_t g_human_array[MAX_HUMANS] = {'\0'};
unsigned int g_human_count = 0;

/* This function reads a list of humans from the file (argument 1) Humans will
 * be stored into the g_human_array. Asserts the argument is not NULL. Returns
 * the number of human records read from the input file.
 */

int read_humans(char * human_file) {

#define CHAR_ZERO ('0')
#define CHAR_NINE ('9')
#define CHAR_APOSTROPHE ('\'')
#define CHAR_HYPHEN ('-')
#define TRUE (1)
#define MAX_STRING_SIZE (100)
#define CHECK_NUM_READ (6)
#define MAX_NAME_LEN (9)

  assert(human_file != NULL);
  FILE * f_read = NULL;
  f_read = fopen(human_file, "r");
  if (f_read == NULL) {
    return NON_READABLE_FILE;
  }

  char temp_last[MAX_STRING_SIZE] = {'\0'};
  char temp_first[MAX_STRING_SIZE] = {'\0'};
  char temp_mother_last[MAX_STRING_SIZE] = {'\0'};
  char temp_mother_first[MAX_STRING_SIZE] = {'\0'};
  char temp_father_last[MAX_STRING_SIZE] = {'\0'};
  char temp_father_first[MAX_STRING_SIZE] = {'\0'};

  g_human_count = 0;
  int num_read = 0;

  /* This while loop is infinite because it is designed to break out when end
   * of the file is reached or an error in the data is found. It starts out by
   * setting the variable num_read equal to the return value from fscanf, which
   * should return 6.
   */

  while (TRUE) {
    num_read = fscanf(f_read, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                      temp_last, temp_first, temp_mother_last,
                      temp_mother_first, temp_father_last,
                      temp_father_first);

    if (num_read == EOF) {
      break;
    }

    if (num_read != CHECK_NUM_READ) {
      fclose(f_read);
      f_read = NULL;
      return READ_BAD_RECORD;
    }

    /* This for loop checks to see if temp_last string has any characters that
     * are not in the alphabet (excluding ' and -). If there are any,
     * READ_BAD_RECORD is returned. If all characters are acceptable, strncpy
     * is used to copy the first 9 characters of temp_last into the last_name
     * array, then terminated with a null character.
     */

    for (int i = 0; i < strlen(temp_last); i++) {
      if (!isalpha(temp_last[i])) {
        if ((temp_last[i] != CHAR_APOSTROPHE) &&
            (temp_last[i] != CHAR_HYPHEN)) {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }
    strncpy(g_human_array[g_human_count].last_name, temp_last, MAX_NAME_LEN);
    g_human_array[g_human_count].last_name[MAX_NAME_LEN] = '\0';

    /* This for loop is the same as the one above, but for temp_first. It copies
     * the first 9 characters of temp_first into the first_name array, then
     * terminated with a null character.
     */

    for (int i = 0; i < strlen(temp_first); i++) {
      if (!isalpha(temp_first[i])) {
        if ((temp_last[i] != CHAR_APOSTROPHE) &&
            (temp_last[i] != CHAR_HYPHEN)) {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }
    strncpy(g_human_array[g_human_count].first_name, temp_first, MAX_NAME_LEN);
    g_human_array[g_human_count].first_name[MAX_NAME_LEN] = '\0';

    /* This for loop is the same as the one above, but for temp_mother_last.
     * It copies the first 9 characters of temp_mother_last into the
     * mother_last array, then terminated with a null character.
     */

    for (int i = 0; i < strlen(temp_mother_last); i++) {
      if (!isalpha(temp_mother_last[i])) {
        if ((temp_mother_last[i] != CHAR_APOSTROPHE) &&
            (temp_mother_last[i] != CHAR_HYPHEN)) {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }
    strncpy(g_human_array[g_human_count].mother_last,
            temp_mother_last, MAX_NAME_LEN);
    g_human_array[g_human_count].mother_last[MAX_NAME_LEN] = '\0';

    /* This for loop is the same as the one above, but for temp_mother_first.
     * It copies the first 9 characters of temp_mother_first into the
     * mother_first array, then terminated with a null character.
     */

    for (int i = 0; i < strlen(temp_mother_first); i++) {
      if (!isalpha(temp_mother_first[i])) {
        if ((temp_mother_first[i] != CHAR_APOSTROPHE) &&
            (temp_mother_first[i] != CHAR_HYPHEN)) {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }
    strncpy(g_human_array[g_human_count].mother_first,
            temp_mother_first, MAX_NAME_LEN);
    g_human_array[g_human_count].mother_first[MAX_NAME_LEN] = '\0';

    /* This for loop is the same as the one above, but for temp_father_last.
     * It copies the first 9 characters of temp_father_last into the
     * father_last array, then terminated with a null character.
     */

    for (int i = 0; i < strlen(temp_father_last); i++) {
      if (!isalpha(temp_father_last[i])) {
        if ((temp_father_last[i] != CHAR_APOSTROPHE) &&
            (temp_father_last[i] != CHAR_HYPHEN)) {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }
    strncpy(g_human_array[g_human_count].father_last,
            temp_father_last, MAX_NAME_LEN);
    g_human_array[g_human_count].father_last[MAX_NAME_LEN] = '\0';

    /* This for loop is the same as the one above, but for temp_father_first.
     * It copies the first 9 characetrs of temp_father_last into the
     * father_last array, then terminated with a null character.
     */

    for (int i = 0; i < strlen(temp_father_first); i++) {
      if (!isalpha(temp_father_first[i])) {
        if ((temp_father_first[i] != CHAR_APOSTROPHE) &&
            (temp_father_first[i] != CHAR_HYPHEN)) {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }
    strncpy(g_human_array[g_human_count].father_first,
            temp_father_first, MAX_NAME_LEN);
    g_human_array[g_human_count].father_first[MAX_NAME_LEN] = '\0';


    /* This group of statements sets the mother, father, child, and sibling
     * pointers to NULL.
     */

    g_human_array[g_human_count].mother_ptr = NULL;
    g_human_array[g_human_count].father_ptr = NULL;
    g_human_array[g_human_count].child_ptr = NULL;
    g_human_array[g_human_count].sibling_ptr = NULL;

    g_human_count++;
  } /* end while loop */

  if (g_human_count > MAX_HUMANS) {
    fclose(f_read);
    f_read = NULL;
    return READ_BAD_RECORD;
  }

  fclose(f_read);
  f_read = NULL;
  return g_human_count;
} /* read_humans() */

/* This function finds the mother, father, children, and siblings for each
 * element. If any of the elements do not have some of the family members,
 * then that family member value is set to NULL.
 */

void establish_family() {

#define EQUAL (0)
  int human_index = 0;
  human_t current_human = g_human_array[human_index];

  while (human_index < g_human_count) {
    current_human = g_human_array[human_index];

    /* This for loop finds the mother of the current human_t.
     * If found, it sets the mother_ptr to the mother's array entry.
     * If not found, mother_ptr is NULL.
     */

    for (int i = 0; i < g_human_count; i++) {
      if (i != human_index) {
        if (strcmp(g_human_array[i].last_name,
            current_human.mother_last) == EQUAL) {
          if (strcmp(g_human_array[i].first_name,
              current_human.mother_first) == EQUAL) {
            g_human_array[human_index].mother_ptr = &g_human_array[i];
          }
        }
      }
    }

    /* This for loop is the same as the one above, but finds the father's array
     * index instead. If there is no matching father entry, father_ptr is NULL.
     */

    for (int i = 0; i < g_human_count; i++) {
      if (i != human_index) {
        if (strcmp(g_human_array[i].first_name,
                   current_human.father_first) == EQUAL) {
          if (strcmp(g_human_array[i].last_name,
                     current_human.father_last) == EQUAL) {
            g_human_array[human_index].father_ptr = &g_human_array[i];
          }
        }
      }
    }

    /* This for loop is the same as the one above, but finds the lowest index
     * child instead. If there are no matching children, child_ptr is NULL.
     */

    for (int i = 0; i < g_human_count; i++) {
      if (i != human_index) {
        if (strcmp(g_human_array[i].father_last,
            current_human.last_name) == EQUAL) {
          if (strcmp(g_human_array[i].father_first,
              current_human.first_name) == EQUAL) {
            g_human_array[human_index].child_ptr = &g_human_array[i];
            break;
          }
        }
        if (strcmp(g_human_array[i].mother_last,
            current_human.last_name) == EQUAL) {
          if (strcmp(g_human_array[i].mother_first,
            current_human.first_name) == EQUAL) {
            g_human_array[human_index].child_ptr = &g_human_array[i];
            break;
          }
        }
      }
    }

    /* This for loop sets a sibling pointer for each entry in the g_human_array.
     * If there are multiple siblings, each sibling must point to a sibling
     * of a higher index. If there is no higher index sibling, point to NULL.
     * If there are no siblings at all, point to NULL.
     */

    for (int i = human_index + 1; i < g_human_count; i++) {
      if (strcmp(g_human_array[i].father_last,
          current_human.father_last) == EQUAL) {
        if (strcmp(g_human_array[i].father_first,
            current_human.father_first) == EQUAL) {
          g_human_array[human_index].sibling_ptr = &g_human_array[i];
          break;
        }
      }
      if (strcmp(g_human_array[i].mother_last,
          current_human.mother_last) == EQUAL) {
        if (strcmp(g_human_array[i].mother_first,
            current_human.mother_first) == EQUAL) {
          g_human_array[human_index].sibling_ptr = &g_human_array[i];
          break;
        }
      }
    }


    human_index++;
  }
} /* establish_family() */
