#ifndef HW9_H
#define HW9_H

typedef struct double_list {
  char            *first_name;
  char            *last_name;
  int              age;
  struct double_list   *prev_ptr;
  struct double_list   *next_ptr;
} double_list;

void create_double ( double_list **, char *, char *, int );
void append_double ( double_list **, double_list * );
void insert_in_order ( double_list **, double_list * );
void remove_double ( double_list **, double_list * );
void delete_double ( double_list ** );
void delete_double_list ( double_list ** );

#endif // HW9_H
