#ifndef HW6_H
#define HW6_H

#define MAX_HUMANS           (1000)
#define NAME_LENGTH            (10)
#define NON_READABLE_FILE (-982375)
#define READ_BAD_RECORD    (-10502)

typedef struct human_struct {
  char father_first[NAME_LENGTH];
  char father_last[NAME_LENGTH];
  char mother_first[NAME_LENGTH];
  char mother_last[NAME_LENGTH];
  char first_name[NAME_LENGTH];
  char last_name[NAME_LENGTH];

  struct human_struct *father_ptr;
  struct human_struct *mother_ptr;
  struct human_struct *child_ptr;
  struct human_struct *sibling_ptr;
} human_t;

extern human_t g_human_array[MAX_HUMANS];
extern unsigned int g_human_count;

int  read_humans(char *);
void establish_family();

#if defined(feof)
#undef feof
#endif

#endif // HW6_H
