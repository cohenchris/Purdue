/* Chris Cohen,
 * hw1.c, CS24000, Fall 2018
 * Last updated August 30, 2018
 */

/* Add any includes here
 */

#include "hw1.h"
#include <stdio.h>

/* This function opens a datafile (parameter 1), reads the
 * records, and calculates the average score of all
 * students enrolled in section_num (parameter 2).
 */

float get_section_average(char *filename, int section_num) {
  FILE * f;
  f = fopen(filename, "r");
  if (f == NULL) {
    return FILE_READ_ERR;
  }

  if ((section_num < 1) || (section_num > 9)) {
    fclose(f);
    f = NULL;
    return BAD_SECTION;
  }

  int count_data = 0;
  int score_total = 0;
  int num_enrolled = 0;
  char name[30];
  int id = 0;
  int score = 0;
  int section = 0;

  /* This while loop uses fscanf. The first parameter is the
   * targeted file, the second is telling the function to
   * stop at the semicolons, and the '%d's are indicative
   * of the filetype that fscanf is taking in. The last 4
   * parameters assign the scanned values to a variable
   * (note that the arguments have to be pointers, which
   * is why '&' is placed in front of the integers, since
   * they aren't pointers by default. fscanf returns the 
   * number of items stored. It then sets that equal to
   * the variable count_data.
   */

  while ((count_data = fscanf(f, "%[^';']; %d; %d%%; %d\n",
                              name, &id, &score, &section))) {
    /* This if statement checks to see if fscanf ran into the
     * end of the file (EOF). If it did, it closes the read
     * pointer, sets it equal to NULL, and breaks.
     */

    if (count_data == EOF) {
      fclose(f);
      f = NULL;
      break;
    }

    /* This if statement checks to see if fscanf ran into
     * a line that didn't have 4 pieces of data. If it did,
     * it closes the read pointer, sets it equal to NULL,
     * and returns BAD_RECORD.
     */

    if (count_data != 4) {
      fclose(f);
      f = NULL;
      return BAD_RECORD;
    }

    if (section == section_num) {
      num_enrolled++;
      score_total += score;
    }
  }

  if (num_enrolled == 0) {
    return NO_STUDENTS;
  }

  /* This return statement divides the total score of all
   * of the students by the number of students enrolled,
   * which gives the average score for the class.
   */

  return (float)score_total / (float)num_enrolled;
} /* get_section_average() */

/* This function will open a datafile (passed through with
 * the parameter), read through the records, and return
 * the maximum final score out of all sections.
 */

int get_top_score(char *filename) {
  FILE * f;
  f = fopen(filename, "r");

  if (f == NULL) {
    return FILE_READ_ERR;
  }

  int count_data = 0;
  int max_score = 0;
  char name[30];
  int id = 0;
  int score = 0;
  int section = 0;
  int num_enrolled = 0;

  /* This while loop uses fscanf. The first parameter is the
   * targeted file, the second is telling the function to
   * stop at the semicolons, and the '%d's are indicative
   * of the filetype that fscanf is taking in. The last 4
   * parameters assign the scanned values to a variable
   * (note that the arguments have to be pointers, which
   * is why '&' is placed in front of the integers, since
   * they aren't pointers by default. fscanf returns the 
   * number of items stored. It then sets that equal to
   * the variable count_data.
   */

  while ((count_data = fscanf(f, "%[^';']; %d; %d%%; %d\n",
                              name, &id, &score, &section))) {
    /* This if statement checks to see if fscanf ran into the
     * end of the file (EOF). If it did, it closes the read
     * pointer, sets it equal to NULL, and breaks.
     */

    if (count_data == EOF) {
      fclose(f);
      f = NULL;
      break;
    }

    /* This if statement checks to see if fscanf ran into
     * a line that didn't have 4 pieces of data. If it did,
     * it closes the read pointer, sets it equal to NULL,
     * and returns BAD_RECORD.
    */

    if (count_data != 4) {
      fclose(f);
      f = NULL;
      return BAD_RECORD;
    }
    if (score > max_score) {
      num_enrolled++;
      max_score = score;
    }
  }

  if (num_enrolled == 0) {
    return NO_STUDENTS;
  }

  return max_score;
} /* get_top_score() */

/* This function will read the in_file (parameter 1) and determine
 * whether the final score of a student is below the cutoff
 * (parameter 2). If it is, the section number, followed by " - ",
 * followed by the student id, followed by " - ", followed by their
 * name will be written to out_file (parameter 3).
 */

int create_fail_list(char *in_file, char *out_file, int cutoff) {
  if ((cutoff <= 0) || (cutoff > 100)) {
    return BAD_CUTOFF;
  }

  FILE * f_in;
  FILE * f_out;
  FILE * f_append;

  f_in = fopen(in_file, "r");
  if (f_in == NULL) {
    return FILE_READ_ERR;
  }

  f_out = fopen(out_file, "w");
  if (f_out == NULL) {
    fclose(f_in);
    f_in = NULL;
    return FILE_WRITE_ERR;
  }
  fclose(f_out);

  f_append = fopen(out_file, "a");
  if (f_out == NULL) {
    fclose(f_in);
    f_in = NULL;
    return FILE_WRITE_ERR;
  }

  int count_data = 0;
  char name[30];
  int id = 0;
  int score = 0;
  int section = 0;
  int num_failed = 0;
  int num_enrolled = 0;

  /* This while loop uses fscanf. The first parameter is the
   * targeted file, the second is telling the function to
   * stop at the semicolons, and the '%d's are indicative
   * of the filetype that fscanf is taking in. The last 4
   * parameters assign the scanned values to a variable
   * (note that the arguments have to be pointers, which
   * is why '&' is placed in front of the integers, since
   * they aren't pointers by default. fscanf returns the 
   * number of items stored. It then sets that equal to 
   * the variable count_data
   */

  while ((count_data = fscanf(f_in, "%[^';']; %d; %d%%; %d\n",
                              name, &id, &score, &section))) {
    /* This if statement checks to see if fscanf ran into the
     * end of the file (EOF). If it did, it closes the read and
     * append pointers, sets each equal to NULL, and breaks.
     */

    if (count_data == EOF) {
      fclose(f_in);
      fclose(f_append);
      f_in = NULL;
      f_append = NULL;
      break;
    }

    /* This if statement checks to see if fscanf ran into
     * a line that didn't have 4 pieces of data. If it did,
     * it closes the read and append pointers, sets them
     * equal to NULL, then returns BAD_RECORD.
     */

    if (count_data != 4) {
      fclose(f_in);
      fclose(f_append);
      f_in = NULL;
      f_append = NULL;
      return BAD_RECORD;
    }

    if (score < cutoff) {
      num_enrolled++;
      num_failed++;
      fprintf(f_append, "%d - %d - %s\n", section, id, name);
    }
  }

  if (num_enrolled == 0) {
    return NO_STUDENTS;
  }

  f_out = NULL;
  return num_failed;
} /* create_fail_list() */

/* This function will read all records in in_file
 * (parameter 1), and if a student is enrolled in
 * section_num (parameter 3), it will determine their
 * final grade and write "student_id: letter_grade" to
 * the out_file (parameter 2). The letter grade cutoffs
 * are:
 * A >= 90
 * B >= 80
 * C >= 70
 * D >= 60
 * F < 60
 */

int create_grade_report(char *in_file, char *out_file, int section_num) {
  if ((section_num < 1) || (section_num > 9)) {
    return BAD_SECTION;
  }

  FILE * f_in;
  FILE * f_out;
  FILE * f_append;

  f_in = fopen(in_file, "r");
  if (f_in == NULL) {
    return FILE_READ_ERR;
  }

  f_out = fopen(out_file, "w");
  if (f_out == NULL) {
    fclose(f_in);
    f_in = NULL;
    return FILE_WRITE_ERR;
  }
  fclose(f_out);

  f_append = fopen(out_file, "a");
  if (f_out == NULL) {
    fclose(f_in);
    fclose(f_out);
    f_in = NULL;
    f_out = NULL;
    return FILE_WRITE_ERR;
  }

  int count_data = 0;
  char name[30];
  int id = 0;
  int score = 0;
  int section = 0;
  int num_enrolled = 0;

  /* This while loop uses fscanf. The first parameter is the
   * targeted file, the second is telling the function to
   * stop at the semicolons, and the '%d's are indicative
   * of the filetype that fscanf is taking in. The last 4
   * parameters assign the scanned values to a variable
   * (note that the arguments have to be pointers, which
   * is why '&' is placed in front of the integers, since
   * they aren't pointers by default. fscanf returns the 
   * number of items stored. It then sets that equal to
   * the variable count_data.
   */

  while ((count_data = fscanf(f_in, "%[^';']; %d; %d%%; %d\n",
                              name, &id, &score, &section))) {
    /* This if statement checks to see if fscanf ran into the
     * end of the file (EOF). If it did, then it closes the
     * read and append pointers, sets each equal to NULL, and
     * breaks.
     */

    if (count_data == EOF) {
      fclose(f_in);
      fclose(f_append);
      f_in = NULL;
      f_append = NULL;
      break;
    }

    /* This if statement checks to see if fscanf ran into
     * a line that didn't have 4 pieces of data. If it did,
     * it closes the read and append pointers, sets them
     * equal to NULL, then returns BAD_RECORD.
     */

    if (count_data != 4) {
      fclose(f_in);
      fclose(f_append);
      f_in = NULL;
      f_append = NULL;
      return BAD_RECORD;
    }

    /* This if statement checks to see if the section number
     * of the current student matches the section number
     * passed as parameter 3. If it is, it increases the
     * enrollment number by 1, then checks to see which letter
     * grade they will have (The guideline for letter grade
     * cutoffs is in the function header). It then appends
     * "id: letter_grade" to the out_file (parameter 2).
     */

    if (section == section_num) {

      num_enrolled++;

      if (score >= 90) {
        fprintf(f_append, "%d: A\n", id);
      }
      else if ((score >= 80) && (score < 90)) {
        fprintf(f_append, "%d: B\n", id);
      }
      else if ((score >= 70) && (score < 80)) {
        fprintf(f_append, "%d: C\n", id);
      }
      else if ((score >= 60) && (score < 70)) {
        fprintf(f_append, "%d: D\n", id);
      }
      else {
        fprintf(f_append, "%d: F\n", id);
      }
    }
  }
  if (num_enrolled == 0) {
    return NO_STUDENTS;
  }

  f_out = NULL;
  return num_enrolled;
} /* create_grade_report() */

/* Remember, you don't need a main function!
 * it is provided by hw1_main.c or hw1_test.
 */

