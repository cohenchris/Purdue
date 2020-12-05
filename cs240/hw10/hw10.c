/*
 * Name, hw10.c, CS 24000, Fall 2018
 * Last updated November 5, 2018
 */

#include "hw10.h"

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

/* This function dynamically allocates memory for a tree structure.
 * Argument 1 is passed in as the name field of the tree struct,
 * argument 2 is passed in as the title field, and argument 3 is
 * passed in as the id_number. left_ptr and right_ptr are set to
 * NULL.
 */

tree * tree_create(char * name, char * title, unsigned int id) {
  assert(name != NULL);
  assert(title != NULL);

  /* Memory is allocated for the new_tree tree based on the
   * sizeof(tree).
   */

  tree * new_tree = malloc(sizeof(tree));
  assert(new_tree != NULL);

  /* Memory is allocated for the name field in new_tree based
   * on the length of argument 1 + 1 (to account for NULL char)
   * and the sizeof a character.
   */

  new_tree->name = malloc((strlen(name) + 1) * sizeof(char));
  assert(new_tree->name != NULL);
  strcpy(new_tree->name, name);

  /* Memory is allocated for the title field in new_tree based
   * on the length of argument 2 + 1 (to account for NULL char)
   * and the sizeof a character.
   */

  new_tree->title = malloc((strlen(title) + 1) * sizeof(char));
  assert(new_tree->title != NULL);
  strcpy(new_tree->title, title);

  new_tree->id_number = id;
  new_tree->left_ptr = NULL;
  new_tree->right_ptr = NULL;

  return new_tree;
}  /* tree_create() */

/* This function inserts argument 2 into the list pointed to by
 * argument 1 in a recursive manner.
 */

void tree_insert(tree ** root, tree * insert_tree) {
  assert(root != NULL);
  assert(insert_tree != NULL);
  assert(insert_tree->left_ptr == NULL);
  assert(insert_tree->right_ptr == NULL);

  if (*root == NULL) {
    (*root) = insert_tree;
  }

  else {
    void my_tree_insert(tree *, tree *);
    my_tree_insert(*root, insert_tree);
  }

} /* tree_insert() */

/* This function inserts argument 2 into the argument 1 based on
 * the name fields.
 */

void my_tree_insert(tree * root, tree * insert_tree) {
  /* The function figures out where to put the new tree based
   * on the result of strcmp(insert_tree->name, root->name).
   * 'Lower' strings go to the left, and 'higher' strings
   * go to the right.
   */

  if (strcmp(insert_tree->name, root->name) <= 0) {
    if (root->left_ptr == NULL) {
      root->left_ptr = insert_tree;
    }
    else {
      my_tree_insert(root->left_ptr, insert_tree);
    }
  }

  else {
    if (root->right_ptr == NULL) {
      root->right_ptr = insert_tree;
    }
    else {
      my_tree_insert(root->right_ptr, insert_tree);
    }
  }
} /* my_tree_insert() */

/* This function recursively searches for a tree node whose name
 * field matches argument 2. If a matching node is found, the
 * function should return a pointer to the node. If no match is
 * found, the function will return NULL.
 */

tree * tree_find(tree * root, char * name_compare) {
  assert(name_compare != NULL);

  if (root == NULL) {
    return NULL;
  }

  if (strcmp(name_compare, root->name) == 0) {  /* Tree found! */
    return root;
  }

  /* A higher node has been found, navigate to the next lower
   * node.
   */

  if (strcmp(name_compare, root->name) < 0) {
    return tree_find(root->left_ptr, name_compare);
  }

  return tree_find(root->right_ptr, name_compare);
} /* tree_find() */

/* This function deletes the entire tree and its name and title
 * fields RECURSIVELY. After the function returns, the root
 * of the tree in argument 1 will be set to NULL.
 */

void tree_delete(tree ** root) {
  assert(root != NULL);

  if (*root == NULL) {
    return;
  }
  tree_delete(&((*root)->left_ptr));
  tree_delete(&((*root)->right_ptr));

  free((*root)->name);
  (*root)->name = NULL;

  free((*root)->title);
  (*root)->title = NULL;

  free(*root);
  (*root) = NULL;
} /* tree_delete() */
