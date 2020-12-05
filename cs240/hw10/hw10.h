#ifndef HW10_H
#define HW10_H

typedef struct tree {
  struct tree  *left_ptr;
  struct tree  *right_ptr;
  char         *name;
  char         *title;
  unsigned int  id_number;
} tree;

tree *tree_create(char *, char *, unsigned int);
void  tree_insert(tree **, tree *);
tree *tree_find(tree *, char *);
void  tree_delete(tree **);

#endif // HW10_H
