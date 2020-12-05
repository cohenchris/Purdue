#ifndef HW5_H
#define HW5_H

#define MAX_HUMANS          (1000)
#define NAME_LENGTH           (10)
#define NON_READABLE_FILE (-23459)
#define READ_BAD_RECORD   (-23458)

typedef struct {
  char           father_first[NAME_LENGTH];
  char           father_last[NAME_LENGTH];
  char           mother_first[NAME_LENGTH];
  char           mother_last[NAME_LENGTH];
  char           first_name[NAME_LENGTH];
  char           last_name[NAME_LENGTH];
} human_t;

extern human_t g_human_array[MAX_HUMANS];
extern unsigned int g_human_count;

int read_humans(char *);
int find_children(char *, char *, int *);

#endif // HW5_H
