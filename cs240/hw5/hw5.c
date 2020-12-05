/*
 * Chris Cohen, hw5.c, CS 24000, Fall 2018
 * Last updated October 3, 2018
 */

#include "hw5.h"

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
    strncpy(g_human_array[g_human_count].last_name, temp_last, 9);
    g_human_array[g_human_count].last_name[9] = '\0';

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
    strncpy(g_human_array[g_human_count].first_name, temp_first, 9);
    g_human_array[g_human_count].first_name[9] = '\0';

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
    strncpy(g_human_array[g_human_count].mother_last, temp_mother_last, 9);
    g_human_array[g_human_count].mother_last[9] = '\0';

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
    strncpy(g_human_array[g_human_count].mother_first, temp_mother_first, 9);
    g_human_array[g_human_count].mother_first[9] = '\0';

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
    strncpy(g_human_array[g_human_count].father_last, temp_father_last, 9);
    g_human_array[g_human_count].father_last[9] = '\0';

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
    strncpy(g_human_array[g_human_count].father_first, temp_father_first, 9);
    g_human_array[g_human_count].father_first[9] = '\0';

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

/* This function finds all instances of human_t's where the full names of either
 * the mother of the father match the seach arguments. Stores the array index
 * of a matching human in the array pointed to by the child_array argument.
 * Returns the number of children found, or if no parents are found, returns 0.
 */

int find_children(char *last_name, char *first_name, int *child_array) {

#define EQUAL (0)


  assert(last_name != NULL);
  assert(first_name != NULL);
  assert(child_array != NULL);

  int father_last_comp = 0;
  int father_first_comp = 0;
  int mother_last_comp = 0;
  int mother_first_comp = 0;
  int child_array_index = 0;

  /* This for loop sets the father_last_comp and father_first_comp variables
   * equal to the return value from strncmp. If the father last & first names
   * in the array equals the last & first name in the parameters, then the index
   * of that human is added to the child_array, starting from zero. The
   * child_array_index is incremented every time a new child is added.
   * The second for loop is the same as this one except it compares the
   * mother first & last names with the ones in the parameters.
   */

  for (int i = 0; i < g_human_count; i++) {
    father_last_comp = strncmp(g_human_array[i].father_last, last_name, 9);
    father_first_comp = strncmp(g_human_array[i].father_first, first_name, 9);
    if ((father_last_comp == EQUAL) &&
        (father_first_comp == EQUAL)) {
      child_array[child_array_index] = i;
      child_array_index++;
    }

    mother_last_comp = strncmp(g_human_array[i].mother_last, last_name, 9);
    mother_first_comp = strncmp(g_human_array[i].mother_first, first_name, 9);
    if ((mother_last_comp == EQUAL) &&
        (mother_first_comp == EQUAL)) {
      child_array[child_array_index] = i;
      child_array_index++;
    }
    father_last_comp = 0;
    father_first_comp = 0;
    mother_last_comp = 0;
    mother_first_comp = 0;
  }

  return child_array_index;
} /* find_children() */
