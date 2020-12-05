#ifndef HW1_H
#define HW1_H

/* Constant Definitions */

#define MAXFILELEN      (20)  /* max file name length                   */
#define MAX_NAME_LEN    (30)  /* max length of name string              */

/* Error Codes */

#define OK              (0)   /* No errors, everything as should be     */
#define BAD_RECORD     (-3)   /* unexpected fields/chars in record      */
#define FILE_READ_ERR  (-4)   /* can't open file for read               */
#define FILE_WRITE_ERR (-5)   /* can't open file for write              */
#define BAD_SECTION    (-6)   /* invalid section number		      */
#define BAD_CUTOFF     (-7)   /* invalid cutoff number		      */
#define NO_STUDENTS    (-8)   /* no students are available to process   */

#define access(x,y) (printf("\n\n\n\n\nDo not use access().\n"), abort(), 1)

#include <stdlib.h>

/* Function Prototypes */

float get_section_average(char *, int);
int get_top_score(char *);
int create_fail_list(char *, char *, int);
int create_grade_report(char *, char *, int);

#endif // HW1_H
