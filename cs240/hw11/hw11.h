#ifndef HW11_H
#define HW11_H

#define STUDENT (1)
#define PERSON  (2)

/* Generic node structure */

struct node {                                /* generic tree node           */
  struct node *left;                         /* pointer to left child       */
  struct node *right;                        /* pointer to right child      */
  void        *data;                         /* pointer to the data struct  */
  void        (*print)(void *);              /* pointer to print function   */
  void        (*delete)(void **);            /* pointer to delete function  */
  int         (*compare)(void *, void *);    /* pointer to compare function */
};

/* person structure */

struct person {                              /* data struct 1:person struct */
  char  *first_name;
  char  *last_name;
  char  *occupation;
  int    age;
  int    ssn;                                /* comparison field */
};

/* student structure */

struct student {
  char  *first_name;
  char  *last_name;
  char  *college;
  float  gpa;
  int    id;                                 /* comparison field */
};

/*
 * Functions of the tree
 */

void print_tree(struct node *);
void delete_tree(struct node **);

/*
 * Functions of the tree-nodes
 */

void print_node(struct node *);
void create_node(struct node **, void *, void (*)(void*),
                 void (*)(void **), int (*)(void *, void *));
void delete_node(struct node **);
void insert_node(struct node **, struct node *);
void remove_node(struct node **, struct node *);
struct node **find_nodes(struct node *, void *, int *);

/*
 * Functions for accessing/manipulating node-data
 */

/* data access/manipulation functions for person structure */

void print_person_data(void *);
void create_person_data(void **, const char *,
                        const char *, const char *, int, int);
void delete_person_data(void **);
int  compare_person_data(void *, void *);

/* data access/manipulation functions for student structure */

void print_student_data(void *);
void create_student_data(void **, const char *,
                         const char *, const char *, float, int);
void delete_student_data(void **);
int  compare_student_data(void *, void *);

#endif // HW11_H
