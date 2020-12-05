#ifndef HW4_H
#define HW4_H

#define NON_READABLE_FILE (-3458729)
#define NON_WRITABLE_FILE (-987424)
#define READ_BAD_RECORD   (-299494)
#define NOT_FOUND         (-828901)
#define NOT_SCHEDULABLE   (-12985)

#define FIRST_NAME_SIZE    (20)
#define LAST_NAME_SIZE     (20)
#define MAX_STUDENTS       (1000)
#define DORMS              (1500)
#define DAYS               (5)
#define HOURS              (10)

#define MONDAY             (0)
#define TUESDAY            (1)
#define WEDNESDAY          (2)
#define THURSDAY           (3)
#define FRIDAY             (4)

#define HOUR_7             (0)
#define HOUR_8             (1)
#define HOUR_9             (2)
#define HOUR_10            (3)
#define HOUR_11            (4)
#define HOUR_12            (5)
#define HOUR_13            (6)
#define HOUR_14            (7)
#define HOUR_15            (8)
#define HOUR_16            (9)

enum u_or_g { UNDERGRADUATE, GRADUATE };

typedef struct {
  unsigned int   id_number;
  char           first_name[FIRST_NAME_SIZE];
  char           last_name[LAST_NAME_SIZE];
  enum u_or_g    classification;
  float          money;
  unsigned short dorm_room;
  char           schedule[DAYS][HOURS];
} student_t;

extern student_t g_student_array[MAX_STUDENTS];
extern unsigned int g_student_count;

int read_students ( char * );
int write_students ( char *, int );
int find_student_by_id ( unsigned int );
int find_student_by_name ( char *, char * );
float compute_undergraduate_percent ();
int grant_scholarship ();
int find_full_dorm_room ();
unsigned int find_earliest_meeting ( unsigned int, unsigned int );

#endif // HW4_H
