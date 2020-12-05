/*
 * Chris Cohen, hw4.c, CS 24000, Fall 2018
 * Last updated September 21, 2018
 */

#include "hw4.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

student_t g_student_array[MAX_STUDENTS] = {'\0'};
unsigned int g_student_count = 0;

/* This function reads a list of students from the file (argument 1)
 * and stores the students into g_student_array. The function
 * returns the amount of students read and sets g_student_count
 * equal to the number
 */

int read_students(char *student_list) {

#define CHECK_NUM_READ (7)
#define MAX_SCHEDULE_LENGTH (50)
#define CHAR_ZERO ('0')
#define CHAR_ONE ('1')
#define CHAR_NINE ('9')
#define CHAR_DECIMAL ('.')
#define MIN_MONEY (0.0)
#define MIN_DORM (0)
#define TRUE (1)
#define MIN_STR_LENGTH (1)

  assert(student_list != NULL);

  FILE * f_read = NULL;
  f_read = fopen(student_list, "r");
  if (f_read == NULL) {
    return NON_READABLE_FILE;
  }

  g_student_count = 0;
  int num_read_in = 0;
  int num_students_read = 0;
  char schedule_string[51] = {'\0'};
  char classification_char = '\0';

  /* This while loop first reads in each line from the input file and stores
   * values in their respective places in the array. it sets the variable
   * 'num_read_in' to the number of variables in each line that have
   * been read in. The loop is infinite because it will read until
   * the end of the file or until it hits an invalid line.
   */

  while (TRUE) {
    (num_read_in = fscanf(f_read, "%u,%[^,],%[^,],%c,$%f,%hu,%[^\n]",
                          &g_student_array[g_student_count].id_number,
                          g_student_array[g_student_count].last_name,
                          g_student_array[g_student_count].first_name,
                          &classification_char,
                          &g_student_array[g_student_count].money,
                          &g_student_array[g_student_count].dorm_room,
                          schedule_string));
    if (num_read_in == EOF) {
      break;
    }

    if (num_read_in != CHECK_NUM_READ) {
      fclose(f_read);
      f_read = NULL;
      return READ_BAD_RECORD;
    }


    if (num_read_in != EOF) {
      num_students_read++;
    }

    /* If the length of the schedule string (consisting of 1s and 0s) is
     * not 50 which would give a full week, READ_BAD_RECORD is returned.
     * The schedule string is then stored in the g_student_array.
     */

    if (strlen(schedule_string) != MAX_SCHEDULE_LENGTH) {
      fclose(f_read);
      f_read = NULL;
      return READ_BAD_RECORD;
    }
    for (int i = MONDAY; i <= FRIDAY; i++) {
      for (int j = HOUR_7; j <= HOUR_16; j++) {
        if (schedule_string[HOURS * i + j] == CHAR_ZERO) {
          g_student_array[g_student_count].schedule[i][j] = 0;
        }
        else if (schedule_string[HOURS * i + j] == CHAR_ONE) {
          g_student_array[g_student_count].schedule[i][j] = 1;
        }
        else {
          fclose(f_read);
          f_read = NULL;
          return READ_BAD_RECORD;
        }
      }
    }

    /* This classification enum is assigned to each student based on
     * which character was present in the data read in. If neither
     * U or G is present, READ_BAD_RECORD is returned.
     */

    if (classification_char == 'U') {
      g_student_array[g_student_count].classification = UNDERGRADUATE;
    }
    else if (classification_char == 'G') {
      g_student_array[g_student_count].classification = GRADUATE;
    }
    else {
      fclose(f_read);
      f_read = NULL;
      return READ_BAD_RECORD;
    }

    /* If the dorm room of the student is greater than DORMS or
     * less than MIN_DORM, READ_BAD_RECORD is returned. Also,
     * if a students' money is less than MIN_MONEY, READD_BAD_RECORD
     * is returned.
     */

    if ((g_student_array[g_student_count].dorm_room > DORMS) ||
        (g_student_array[g_student_count].dorm_room < MIN_DORM)) {
      return READ_BAD_RECORD;
    }
    if (g_student_array[g_student_count].money < MIN_MONEY) {
      return READ_BAD_RECORD;
    }
    g_student_count++;
  } /* end while loop */

  /* If there are more students read in than MAX_STUDENTS,
   * READ_BAD_RECORD is returned.
   */

  if (g_student_count >= MAX_STUDENTS) {
    return READ_BAD_RECORD;
  }

  fclose(f_read);
  f_read = NULL;
  g_student_count = num_students_read;
  return num_students_read;
} /* read_students() */

/* This function will write a list of students to the file specified
 * by argument 1. Argument 2 specifies the number of entries from
 * g_student_array to write, and the function should write that many.
 */

int write_students(char *student_write_file, int num_entries) {

#define SCHEDULE_ZERO (0)
#define SCHEDULE_ONE (1)

  assert(student_write_file != NULL);
  assert(num_entries >= 0);

  FILE * f_write = NULL;
  f_write = fopen(student_write_file, "w");
  if (f_write == NULL) {
    return NON_WRITABLE_FILE;
  }
  fclose(f_write);

  FILE * f_append = NULL;
  f_append = fopen(student_write_file, "a");
  if (f_append == NULL) {
    return NON_WRITABLE_FILE;
  }

  /* This for loop iterates num_entries times (argument 2). It starts
   * by recreating the schedule string by going through the
   * schedule array and creating a string of SCHEDULE_ZERO
   * and SCHEDULE_ONE characters. The string is then appended with a
   * null character.
   */

  for (int i = 0; i < num_entries; i++) {
    char classification_char = '\0';
    char schedule_string[51] = {'\0'};
    for (int j = MONDAY; j <= FRIDAY; j++) {
      for (int k = HOUR_7; k <= HOUR_16; k++) {
        if (g_student_array[i].schedule[j][k] == SCHEDULE_ZERO) {
          schedule_string[HOURS * j + k] = '0';
        }
        else if (g_student_array[i].schedule[j][k] == SCHEDULE_ONE) {
          schedule_string[HOURS * j + k] = '1';
        }
      }
    }
    schedule_string[50] = '\0';

    /* This if statement assignes a character based on the
     * enum value of the studentn.
     */

    if (g_student_array[i].classification == UNDERGRADUATE) {
      classification_char = 'U';
    }
    else {
      classification_char = 'G';
    }

    /* This fprintf statement appends student info to the output file
     * (argument 1) line by line.
     */

    fprintf(f_append, "%u,%s,%s,%c,$%.2f,%hu,%s\n",
            g_student_array[i].id_number,
            g_student_array[i].last_name,
            g_student_array[i].first_name,
            classification_char,
            g_student_array[i].money,
            g_student_array[i].dorm_room,
            schedule_string);
  }
  fclose(f_append);
  f_append = NULL;
  return num_entries;
} /* write_students() */

/* This function will search the array for a student_t in
 * g_student_array whose ID matches the value of the argument.
 * Returns the array index of that studnet_t in the array.
 */

int find_student_by_id(unsigned int id) {
  for (int i = 0; i < g_student_count; i++) {
    if (g_student_array[i].id_number == id) {
      return i;
    }
  }
  return NOT_FOUND;
} /* find_student_by_id() */

/* This function will search for a student_t in g_student_array
 * whose first name matches argument 1, and whose last name matches
 * argument 2. Returns the index in the array of said student.
 */

int find_student_by_name(char *first_name, char *last_name) {
  assert(first_name != NULL);
  assert(last_name != NULL);
  int strcmp_result = 0;

  for (int i = 0; i < g_student_count; i++) {
    strcmp_result = strcmp(g_student_array[i].first_name, first_name);
    if (strcmp_result == 0) {
      strcmp_result = strcmp(g_student_array[i].last_name, last_name);
      if (strcmp_result == 0) {
        return i;
      }
    }
  }
  return NOT_FOUND;
} /* find_student_by_name() */

/* This function should return the percentage of the student body
 * that is classified as undergraduate.
 */

float compute_undergraduate_percent() {
  int num_undergrads = 0;
  for (int i = 0; i < g_student_count; i++) {
    if (g_student_array[i].classification == UNDERGRADUATE) {
      num_undergrads++;
    }
  }
  return (((float)(num_undergrads))/g_student_count) * 100;
} /* compute_undergraduate_percent() */

/* This function should find the member of the student body with the
 * least amount of money, subtract one dollar from everyone's account,
 * and give it to the poorest student. If 2 or more students are tied
 * for being the poorest, returns NOT_FOUND. Otherwise, returns index
 * of the poorest student in g_student_array.
 */

int grant_scholarship() {

#define TIED (tied_poorest > 0)

  float least_money = g_student_array[0].money;
  int index_poorest_student = 0;
  int tied_poorest = 0;

  /* This for loop increments through g_student_array and looks to see
   * if the amount of money for a given student is less than the current
   * least amount. If it is, then least_money is set to that new value
   * and the index i is saved into index_poorest_student. If the money
   * value matches the current lowest, tied_poorest is incremented.
   */

  for (int i = 1; i < g_student_count; i++) {
    if (g_student_array[i].money < least_money) {
      least_money = g_student_array[i].money;
      index_poorest_student = i;
      tied_poorest = 0;
    }

    else if (g_student_array[i].money == least_money) {
      tied_poorest++;
    }
  }

  /* This if statement looks to see if there were multiple students that
   * had the lowest amount of money. If they do, NOT_FOUND is returned.
   * If they don't, then every student except the poorest one has 1 dollar
   * subtracted from their money value, and the poorest student gets
   * g_student_count - 1 extra dollars.
   */

  if (TIED) {
    return NOT_FOUND;
  }

  else {
    for (int i = 0; i < g_student_count; i++) {
      if (i == index_poorest_student) {
        g_student_array[i].money += g_student_count - 1;
      }

      else {
        g_student_array[i].money -= 1;
      }
    }
  }
  return index_poorest_student;
} /* grant_scholarship() */

/* This function should scan through the student body database and find
 * a dorm room that contains 3 or more people. If no such room is found,
 * it should return the NOT_FOUND error. If such a room is found, it
 * should return the room number. If multiple full dorm rooms are found,
 * should return the lowest numbered dorm.
 */

int find_full_dorm_room() {

#define FULL_DORM (3)

  int num_people = 0;
  for (int i = 0; i < DORMS; i++) {
    for (int j = 0; j < g_student_count; j++) {
      if (g_student_array[j].dorm_room == i) {
        num_people++;
      }
    }
    if (num_people >= FULL_DORM) {
      return i;
    }
    num_people = 0;
  }
  return NOT_FOUND;
} /* find_full_dorm_room() */

/* This function will accept the ID numbers of 2 students and find the
 * first time during the week that neither of them have a schedule
 * conflict. If either student isn't found, should return NOT_FOUND.
 * If they don't have any free time in common, should return
 * NOT_SCHEDULABLE. Otherwise, returns an int that represents the
 * day of the week (M-F) times 100 plus the hour of the day (7-16).
 */

unsigned int find_earliest_meeting(unsigned int id_one, unsigned int id_two) {

#define NO_STUDENT (-1)
#define FREE_SCHED (0)

  int index_student_one = NO_STUDENT;
  int index_student_two = NO_STUDENT;

  /* These next two for loops look for each id in g_student_array and save
   * the index of each for later use. If either one is not found, NOT_FOUND
   * is returned.
   */

  for (int i = 0; i < g_student_count; i++) {
    if (g_student_array[i].id_number == id_one) {
      index_student_one = i;
    }
  }

  for (int i = 0; i < g_student_count; i++) {
    if (g_student_array[i].id_number == id_two) {
      index_student_two = i;
    }
  }

  if ((index_student_one == NO_STUDENT) ||
      (index_student_two == NO_STUDENT)) {
    return NOT_FOUND;
  }

  /* Each hour of the week is looked through, and if each student has a '0'
   * during the time period, then that value is returned in the format mentioned
   * in the function header. If no opening found, NOT_SCHEDULABLE is returned.
   */

  for (int i = MONDAY; i <= FRIDAY; i++) {
    for (int j = HOUR_7; j <= HOUR_16; j++) {
      if ((g_student_array[index_student_one].schedule[i][j] == FREE_SCHED) &&
          (g_student_array[index_student_two].schedule[i][j] == FREE_SCHED)) {
        return (100 * i) + j + 7;
      }
    }
  }

  return NOT_SCHEDULABLE;
} /* find_earliest_meeting() */
