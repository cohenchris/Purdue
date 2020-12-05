#include <stdio.h>
#include <unistd.h>

#include "hw10.h"

/*
 *  In case you're curious, we need to include unistd.h to declare
 *  the sleep() function.
 */

#define QUIT (0)
#define INSERT (1)
#define FIND (2)
#define DELETE (3)
#define PRINT (4)

/*
 *  The function prints the name, title, and id_number of the tree passed
 *  into it.
 */

void tree_print_node(tree *tree_ptr) {
  printf("name: %s;\ttitle: %s;\tid: %d\n",
         tree_ptr->name, tree_ptr->title, tree_ptr->id_number);
} /* tree_print_node() */

/*
 *  This function prints the name, title, and id_number of each node
 *  in the binary tree passed to it.
 */

void tree_print(tree *tree_ptr) {
  if (tree_ptr == NULL) {
    return;
  }

  tree_print(tree_ptr->left_ptr);
  tree_print_node(tree_ptr);
  tree_print(tree_ptr->right_ptr);
} /* tree_print() */

/*
 *  This function is used to run the functions in hw10.c. User input is used
 *  to determine which function to run an what is passed to it. main()
 *  prints information related to running the chosen function.
 */

int main() {
  tree *tree_root = NULL;
  tree *tree_node = NULL;

  // Disable stdlib I/O buffering

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) Insert\n"
           "2) Find\n"
           "3) Delete\n"
           "4) Print\n"
           "\nSelect a function: ");

    int selected_option = 333333;
    scanf("%d", &selected_option);

    char name[100] = "\0";
    if ((selected_option > PRINT) || (selected_option < QUIT)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (selected_option == QUIT) {
      printf("\n\n\n\n\n\nHave a nice day.\n");
      return 0;
    }
    else if (selected_option == INSERT) {
      unsigned int id = 0;
      char title[100] = "\0";

      printf ("Enter the name:  ");
      scanf("%s", name);
      printf ("Enter the title: ");
      scanf("%s", title);
      printf ("Enter the id:    ");
      scanf("%d", &id);
      tree_node = tree_create(name, title, id);
      tree_insert(&tree_root, tree_node);
    }

    else if (selected_option == FIND) {
      printf ("Enter a name to search for: ");
      scanf("%s", name);
      tree_node = tree_find(tree_root, name);
      if (tree_node == NULL) {
        printf("Not found.\n");
      }
      else {
        printf("The matching tree element looks like: \n");
        tree_print_node(tree_node);
      }
    }

    else if (selected_option == DELETE) {
      tree_delete(&tree_root);
    }

    else {
      tree_print(tree_root);
    }
  }

  /* not reached. */

  return 0;
} /* main() */

