/*
 * Chris Cohen, hw11.c, CS 24000, Fall 2018
 * Last updated November 15, 2018
 */

#include "hw11.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define START (0)

/////////////////////////////////////////////////
//  TREE  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints out the values stored in a tree given
 *  a pointer to the tree's root node.
 */

void print_tree(struct node * root) {
  if (!root) {
    return;
  }

  print_tree(root->left);
  print_node(root);
  print_tree(root->right);
} /* print_tree() */

/* This function recursively deletes the tree whose root is argument 1.
 * Asserts that argument 1 is not NULL.
 */

void delete_tree(struct node ** root) {
  assert(root != NULL);

  if (*root == NULL) {
    return;
  }

  if ((*root)->left != NULL) {
    delete_tree(&((*root)->left));
  }

  if ((*root)->right != NULL) {
    delete_tree(&((*root)->right));
  }

  delete_node(root);
} /* delete_tree() */

/////////////////////////////////////////////////
//  GENERIC  NODE  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints the values associated with a node's data.
 */

void print_node(struct node *n_ptr) {
  assert(n_ptr && n_ptr->data && n_ptr->print);

  (n_ptr->print)(n_ptr->data);
} /* print_node() */

/* This function dynamically allocates a new node and initializes the
 * pointers of the node using arguments 2, 3, 4, and 5. It asserts
 * that arguments 1-5 are not NULL, and that the pointer of argument
 * 1 is NULL.
 */

void create_node(struct node ** node, void * data, void (*print)(void *),
                 void (*delete)(void **), int (*comp)(void *, void *)) {
  assert(node != NULL);
  assert(data != NULL);
  assert(print != NULL);
  assert(delete != NULL);
  assert(comp != NULL);
  assert(*node == NULL);

  struct node * new = malloc(sizeof(struct node));
  assert(new != NULL);

  new->left = NULL;
  new->right = NULL;
  new->data = data;
  new->print = print;
  new->delete = delete;
  new->compare = comp;

  *node = new;
} /* create_node () */

/* This function deallocates the data field in the pointer of argument
 * 1 by calling the delete function in the pointer of argument 1, then
 * frees the node itself. Asserts that argument 1 and the pointer of
 * argument 1 are not NULL. Also asserts that the left and right
 * pointers of the pointer of argument 1 are NULL.
 */

void delete_node(struct node ** node) {
  assert(node != NULL);
  assert(*node != NULL);
  assert((*node)->left == NULL);
  assert((*node)->right == NULL);

  ((*node)->delete)(&((*node)->data));
  free(*node);
  *node = NULL;
} /* delete_node () */

/* Inserts argument 2 into the tree with root argument 1 based on the
 * return value of the compare function that is stored in the pointer
 * of argument 1. Asserts that arguments 1 and 2 are not NULL.
 */

void insert_node(struct node ** root, struct node * new) {
  assert(root != NULL);
  assert(new != NULL);

  struct node * original_root_val = *root;

  if ((*root) == NULL) {
    (*root) = new;
    return;
  }

  while (*root != NULL) {
    if (((*root)->compare)(((void *)((*root)->data)),
                          ((void *)((new)->data))) >= 0) {
      /* This if statement makes sure that the left node is NULL
       * before adding the new value. If it isn't, it goes left
       * and re-enters the while loop until it finds a node
       * that is NULL.
       */

      if ((*root)->left == NULL) {
        (*root)->left = new;
        (*root) = original_root_val;
        return;
      }
      else {
        (*root) = (*root)->left;
      }
    }

    else if (((*root)->compare)(((void *)((*root)->data)),
                               ((void *)((new)->data))) < 0) {
      /* This if statement makes sure that the right node is NULL
       * before adding the new value. If it isn't it goes right
       * and re-enters the while loop until it finds a node
       * that is NULL.
       */

      if ((*root)->right == NULL) {
        (*root)->right = new;
        (*root) = original_root_val;
        return;
      }
      else {
        (*root) = (*root)->right;
      }
    }
  }
} /* insert_node () */

/* This function recursively iterates through the tree with root argument 1
 * and counts how many nodes have the same data value as the one passed
 * into argument 2.
 */

int count_nodes(struct node * root, void * data) {
  int count = START;

  if (root == NULL) {
    return count;
  }

  if (((root)->compare)(root->data, data) == 0) {
    count++;
  }

  if ((root->left == NULL) &&
      (root->right == NULL)) {
    return count;
  }

  count += count_nodes(root->left, data);
  count += count_nodes(root->right, data);

  return count;
} /* count_nodes() */

/* This function recursively iterates through the tree. If it happens
 * across a node that's data value matches argument 2, it puts the
 * address into the array (argument 3).
 */

void found_nodes(struct node * root, void * data, struct node ** arr, int ind) {
  if (root == NULL) {
    return;
  }

  if (((root)->compare)(root->data, data) == 0) {
    arr[ind] = root;
    ind++;
  }

  if ((root->left == NULL) &&
      (root->right == NULL)) {
    return;
  }

  found_nodes(root->left, data, arr, ind);
  found_nodes(root->right, data, arr, ind);

} /* found_nodes() */

/* This function dynamically allocates memory for an array of struct node
 * pointers and fills it in with the addresses of the matching nodes.
 * Asserts that all arguments are not NULL. Uses count_nodes() and
 * found_nodes().
 */

struct node ** find_nodes(struct node * root, void * data, int * num) {

  assert(root != NULL);
  assert(data != NULL);
  assert(num != NULL);

  *num = count_nodes(root, data);

  if (*num == 0) {
    return NULL;
  }

  struct node ** arr_duplicates = malloc(sizeof(struct node *) * (*num));
  assert(arr_duplicates != NULL);

  int starting_index = START;
  found_nodes(root, data, arr_duplicates, starting_index);
  return arr_duplicates;
} /* find_nodes() */

/* Function header..
 */

void remove_one(struct node ** root, struct node * data) {
  if (*root == NULL) {
    return;
  }

  if (((*root)->compare)((*root)->data, data) == 0) {
   if (((*root)->left != NULL) &&
       ((*root)->right != NULL)) {
     struct node moved_node = (*root)->right;
     (*root) = (*root)->left;
     while ((*root)->right != NULL) {
       (*root) = (*root)->right;
     }
     (*root)->right = moved_node;
     return;  //make function not recursive so it can stop after removing one
   }

   return;
  }

  if (((*root)->left == NULL) &&
      ((*root)->right == NULL)) {
    return;
  }

  remove_one((*root)->left, data);
  remove_one((*root)->right, data);
} /* remove_one() */

/* This function removes nodes from the tree with root argument 1. If
 * the node matches argument 2, it is removed.
 */

void remove_node(struct node ** root, struct node * removed) {
  assert(root != NULL);
  assert(removed != NULL);

  int num_duplicates = count_nodes(*root, (removed)->data);
  printf("%d\n", num_duplicates);

  while (num_duplicates > 0) {
    remove_one(root, removed);
    num_duplicates--;
  }
} /* remove_node() */

/////////////////////////////////////////////////
//  STRUCT  PERSON  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints the values associated with a person.
 */

void print_person_data(void *person) {
  struct person *person_ptr = (struct person *) person;

  assert(person_ptr && person_ptr->first_name &&
         person_ptr->last_name && person_ptr->occupation);

  printf("%15s %15s %15s %4d %9d\n",
         person_ptr->first_name, person_ptr->last_name, person_ptr->occupation,
         person_ptr->age, person_ptr->ssn);
} /* print_person_data() */

/* This function creates a new person struct that has the values
 * in arguments 2-6. Sets the pointer of argument 1 equal to the
 * new person struct. Asserts that arguments 1-4 are not NULL, and
 * that the pointer of argument 1 is NULL.
 */

void create_person_data(void ** person, const char * first, const char * last,
                        const char * occ, int age, int ssn) {
  assert(person != NULL);
  assert(*person == NULL);
  assert(first != NULL);
  assert(last != NULL);
  assert(occ != NULL);

  struct person * new = malloc(sizeof(struct person));
  assert(new != NULL);

  new->first_name = malloc((strlen(first) + 1) * sizeof(char));
  assert(new->first_name != NULL);
  strcpy(new->first_name, first);

  new->last_name = malloc((strlen(last) + 1) * sizeof(char));
  assert(new->last_name != NULL);
  strcpy(new->last_name, last);

  new->occupation = malloc((strlen(occ) + 1) * sizeof(char));
  assert(new->occupation != NULL);
  strcpy(new->occupation, occ);

  new->age = age;
  new->ssn = ssn;

  *person = new;
} /* create_person_data() */

/* This function deallocates memory for the first_name, last_name, and
 * occupation fields for a person struct. Then, frees the whole person
 * struct. Asserts that argument 1 and the pointer of argument 1 is not NULL.
 */

void delete_person_data(void ** node) {
  assert(node != NULL);
  assert(*node != NULL);

  free(((struct person *)*node)->first_name);
  ((struct person *)*node)->first_name = NULL;

  free(((struct person *)*node)->last_name);
  ((struct person *)*node)->last_name = NULL;

  free(((struct person *)*node)->occupation);
  ((struct person *)*node)->occupation = NULL;

  free((struct person *)*node);
  *node = NULL;
} /* create_person_data() */

/* This function compares the ssn fields of the person structures passed in.
 * If the difference between the ssn of argument 1 and argument 2 is positive,
 * 1 is returned. If it's negative, -1 is returned. If otherwise, 0 is returned.
 */

int compare_person_data(void * person1, void * person2) {
  assert(person1 != NULL);
  assert(person2 != NULL);

  if ((((struct person *)person1)->ssn) >
      (((struct person *)person2)->ssn)) {
    return 1;
  }

  else if ((((struct person *)person1)->ssn) <
      (((struct person *)person2)->ssn)) {
    return -1;
  }

  else {
    return 0;
  }
} /* compare_person_data() */

/////////////////////////////////////////////////
//  STRUCT  STUDENT  FUNCTIONS
/////////////////////////////////////////////////

/*
 *  This function prints the values associated with a student.
 */

void print_student_data(void *student) {
  struct student *student_ptr = (struct student *)student;
  assert(student_ptr && student_ptr->first_name &&
         student_ptr->last_name && student_ptr->college);

  printf("%15s %15s %15s %3.2f %9d\n",
         student_ptr->first_name, student_ptr->last_name, student_ptr->college,
         student_ptr->gpa, student_ptr->id);
} /* student_data() */

/* This function creates a new student struct that has the values
 * in arguments 2-6. Sets the pointer of argument 1 equal to the
 * new student struct. Asserts that arguments 1-4 are not NULL, and
 * that the pointer of argument 1 is NULL.
 */

void create_student_data(void ** student, const char * first, const char * last,
                         const char * college, float gpa, int id) {
  assert(student != NULL);
  assert(*student == NULL);
  assert(first != NULL);
  assert(last != NULL);
  assert(college != NULL);

  struct student * new = malloc(sizeof(struct student));
  assert(new != NULL);

  new->first_name = malloc((strlen(first) + 1) * sizeof(char));
  assert(new->first_name != NULL);
  strcpy(new->first_name, first);

  new->last_name = malloc((strlen(last) + 1) * sizeof(char));
  assert(new->last_name != NULL);
  strcpy(new->last_name, last);

  new->college = malloc((strlen(college) + 1) * sizeof(char));
  assert(new->college != NULL);
  strcpy(new->college, college);

  new->gpa = gpa;
  new->id = id;

  *student = new;
} /* create_student_data() */

/* This function deallocates memory for the first_name, last_name, and
 * college fields for a student struct. Then, frees the whole student
 * struct. Asserts that argument 1 and the pointer of argument 1 is not NULL.
 */

void delete_student_data(void ** node) {
  assert(node != NULL);
  assert(*node != NULL);

  free(((struct student *)*node)->first_name);
  ((struct student *)*node)->first_name = NULL;

  free(((struct student *)*node)->last_name);
  ((struct student *)*node)->last_name = NULL;

  free(((struct student *)*node)->college);
  ((struct student *)*node)->college = NULL;

  free((struct student *)*node);
  *node = NULL;
} /* delete_student_data () */

/* This function compares the id fields of the student structures passed in.
 * If the difference between the id of argument 1 and argument 2 is positive,
 * 1 is returned. If it's negative, -1 is returned. If otherwise, 0 is returned.
 */

int compare_student_data(void * student1, void *student2) {
  assert(student1 != NULL);
  assert(student2 != NULL);

  if ((((struct student *)student1)->id) >
      (((struct student *)student2)->id)) {
    return 1;
  }

  else if ((((struct student *)student1)->id) <
      (((struct student *)student2)->id)) {
    return -1;
  }

  else {
    return 0;
  }
} /* compare_student_data() */
