#ifndef HW7_H
#define HW7_H

// definitions

#define YES        (1)
#define NO         (2)
#define REVERSE    (3)
#define DUPLICATE  (4)

// structures

struct node {
  struct node *next;
  char         val;
};

// function prototypes

char *copy_string(char *, int operation);
struct node *string_to_list(char *, int reverse);
struct node *append_char(struct node *, char);
struct node *prepend_char(struct node *, char);
char *list_to_string(struct node *);
void free_list(struct node *);
char *concat_strings(char *, char*);
void append_lists(struct node *, struct node *);

#endif // HW7_H
