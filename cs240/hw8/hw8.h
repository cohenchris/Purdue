#ifndef HW8_H
#define HW8_H

typedef struct single_list {
  char               *first_name;
  char               *last_name;
  int                 age;
  struct single_list *next_ptr;
} single_list;

typedef struct double_list {
  char               *first_name;
  char               *last_name;
  int                 age;
  struct double_list *prev_ptr;
  struct double_list *next_ptr;
} double_list;

single_list *create_single(char *, char *, int);
double_list *create_double(char *, char *, int);
void    append_single(single_list *, single_list *);
void    append_double(double_list *, double_list *);
double_list *single_to_double(single_list *);
void    delete_single(single_list *);
void    delete_double(double_list *);
void    delete_single_list(single_list *);
void    delete_double_list(double_list *);

#endif // HW8_H
