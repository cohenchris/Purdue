#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw11.h"

#define QUIT (0)
#define PERSON_TREE_MANIPULATIONS (1)
#define STUDENT_TREE_MANIPULATIONS (2)
#define CREATE_NODE (1)
#define INSERT_NODE (2)
#define FIND_NODES (3)
#define REMOVE_NODE (4)
#define DELETE_NODE (5)
#define DELETE_TREE (6)
#define PRINT_TREE (7)
#define MAX_STRING_LENGTH (100)

/*
 *  This function is used to run the functions in hw11.c. User input is used
 *  to determine which function to run and what is passed to it. main()
 *  prints information related to running the chosen function.
 */

int main(int argc, char *argv[]) {
  struct node **array_of_duplicates = NULL;
  struct node *root_node = NULL;
  struct node *node = NULL;
  void *data_node = NULL;

  // Disable stdlib I/O buffering

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  while (1) {
    int data_type = 0;
    int user_choice = 0;
    while (!((data_type == PERSON) || (data_type == STUDENT))) {
      printf("\n\n"
             "0. Quit\n"
             "1. person tree manipulations\n"
             "2. student tree manipulations\n");

      user_choice = 0;
      printf("Enter user_choice: ");
      scanf("%d", &user_choice);
      getchar();

      if ((user_choice < QUIT) || (user_choice > STUDENT_TREE_MANIPULATIONS)) {
        printf("Invalid response!\n");
        continue;
      }

      if (user_choice == QUIT) {
        printf("\nHave a fine day.\n\n");
        return 0;
      }
      else if (user_choice == PERSON_TREE_MANIPULATIONS) {
        data_type = PERSON;
      }
      else {
        data_type = STUDENT;
      }
    }

    int exit = 0;
    while (!exit) {
      char last_name[MAX_STRING_LENGTH] = "\0";
      char first_name[MAX_STRING_LENGTH] = "\0";
      char occupation[MAX_STRING_LENGTH] = "\0";
      char college[MAX_STRING_LENGTH] = "\0";
      int age = 0;
      int ssn = 0;
      int id = 0;
      float gpa = 0.0;

      user_choice = 777;
      printf("\n\n(dataType:Comparison) : %s\n\n",
             data_type == PERSON ? "(PERSON:ssn)" : "(STUDENT:id)");
      printf("0. Quit\n"
             "1. create_node\n"
             "2. insert_node\n"
             "3. find_nodes\n"
             "4. remove_node\n"
             "5. delete_node\n"
             "6. delete_tree\n"
             "7. print_tree\n");

      printf("Enter user_choice: ");
      scanf("%d", &user_choice);
      getchar();

      if ((user_choice < QUIT) || (user_choice > PRINT_TREE)) {
        printf("Invalid response!\n");
        continue;
      }

      if (user_choice == QUIT) {
        printf("Cleaning up!\n");
        if (root_node) {
          delete_tree(&root_node);
        }
        if (node) {
          delete_node(&node);
        }
        exit = 1;
      }

      if ((user_choice == CREATE_NODE) && (!node)) {
        printf("First Name: ");
        scanf("%s", first_name);
        getchar();
        printf("Last Name: ");
        scanf("%s", last_name);
        getchar();

        if (data_type == PERSON) {
          printf("Occupation: ");
          scanf("%s", occupation);
          getchar();
          printf("Age: ");
          scanf("%d", &age);
          getchar();
          printf("SSN: ");
          scanf("%d", &ssn);
          getchar();
        }
        else {
          printf("College:");
          scanf("%s", college);
          getchar();
          printf("GPA: ");
          scanf("%f", &gpa);
          getchar();
          printf("ID: ");
          scanf("%d", &id);
          getchar();
        }
      }

      if (((user_choice == FIND_NODES) || (user_choice == REMOVE_NODE)) &&
          (!node)) {
        if (data_type == PERSON) {
          printf("SSN: ");
          scanf("%d", &ssn);
          getchar();
        }
        else {
          printf("ID: ");
          scanf("%d", &id);
          getchar();
        }
      }

      switch (user_choice) {
        case CREATE_NODE: {
          if (node) {
            printf("There's a node created already. Try "
                    "inserting it in the tree or deleting it before "
                    "allocating another node");
            break;
          }

          if (data_type == PERSON) {

            // Create the data node of the generic node

            create_person_data(&data_node, first_name, last_name, occupation,
                               age, ssn);

            // Use the pointer returned by create_person_data to
            // create the generic node with a pointer to the data
            // node created above

            create_node(&node, data_node, print_person_data,
                        delete_person_data, compare_person_data);
          }
          else {
            create_student_data(&data_node, first_name, last_name,
                                college, gpa, id);
            create_node(&node, data_node, print_student_data,
                        delete_student_data, compare_student_data);
          }

          data_node = NULL;
          printf("Created Item is:\n");
          print_node(node);
          break;
        }
        case INSERT_NODE: {
          if (!node) {
            printf("Create a node first!\n");
            break;
          }
          printf("The tree before insertion :\n");
          print_tree(root_node);
          printf("After inserting node:\n");
          print_node(node);
          insert_node(&root_node, node);
          printf("\nThe new tree looks like :\n");
          print_tree(root_node);
          node = NULL;
          break;
        }
        case FIND_NODES: {
          if (!root_node) {
            printf("There is no tree!\n");
            break;
          }

          if (node) {
            printf("There's a node created already. Try "
                   "inserting it in the tree or deleting it before "
                   "allocating another node");
            break;
          }

          if (data_type == PERSON) {

            // Create a node with only the comparison element

            create_person_data(&data_node, "", "", "", 0, ssn);
          }
          else {

            // Create a node with only the comparison element

            create_student_data(&data_node, "", "", "", 0.0, id);
          }

          // Now use that node to find nodes of the tree that
          // match the comparison
          // array_of_duplicates holds pointers to the duplicate nodes

          int nodes = 0;
          array_of_duplicates = find_nodes(root_node, data_node, &nodes);
          printf("Found %d nodes with %s '%d'\n", nodes,
                 data_type == PERSON ? "ssn" : "id",
                 data_type == PERSON ?  ssn  :  id);
          for (int i = 0; i < nodes; i++) {
            printf("%d. ", i + 1);
            print_node(array_of_duplicates[i]);
          }

          // Free the 'dummy' data used to pass the comparison element
          if (data_type == PERSON) {
            delete_person_data(&data_node);
          }
          else {
            delete_student_data(&data_node);
          }
          data_node = NULL;
          if (array_of_duplicates) {
            free(array_of_duplicates);
            array_of_duplicates = NULL;
          }
          break;
        }
        case REMOVE_NODE: {
          if (!root_node) {
            printf("There is no tree!\n");
            break;
          }

          if (node) {
            printf("There's a node created already. Try "
                   "inserting it in the tree or deleting it before "
                   "removing another node");
            break;
          }

          if (data_type == PERSON) {
            // Create a node with only the comparison element
            create_person_data(&data_node, "", "", "", 0, ssn);
            create_node(&node, data_node, print_person_data,
                        delete_person_data, compare_person_data);
          }
          else {
            // Create a node with only the comparison element
            create_student_data(&data_node, "", "", "", 0.0, id);
            create_node(&node, data_node, print_student_data,
                        delete_student_data, compare_student_data);
          }

          // Now use that node to find nodes of the tree that
          // match the comparison
          // array_of_duplicates holds pointers to the duplicate nodes

          int nodes = 0;
          array_of_duplicates = find_nodes(root_node, data_node, &nodes);
          printf("Found %d nodes with %s '%d'\n", nodes,
                 data_type == PERSON ? "ssn" : "id",
                 data_type == PERSON ?  ssn  :  id);
          for (int i = 0; i < nodes; i++) {
            printf("%d. ", i + 1);
            print_node(array_of_duplicates[i]);
          }

          int index = 0;
          if (nodes > 1) {
            printf("Enter the index of student to remove: ");
            scanf("%d", &index);
            getchar();
            index--;
          }
          else if (nodes == 1) {
            // If there's only one element in array, there are no
            // duplicates, only one node at index 0
            index = 0;
          }
          else {
            // No matching nodes, nodes == 0
            printf("No student found!\n");
            break;
          }

          printf("Removing ");
          print_node(array_of_duplicates[index]);
          remove_node(&root_node, array_of_duplicates[index]);
          printf("New tree looks like: \n");
          print_tree(root_node);

          // Free the 'dummy' node used to pass the comparison element
          // this function deallocates both the node and its data
          delete_node(&node);
          data_node = NULL;

          node = array_of_duplicates[index];
          free(array_of_duplicates);
          array_of_duplicates = NULL;

          break;
        }
        case DELETE_NODE: {
          if (!node) {
            printf("You need to remove or create a node first\n");
            break;
          }
          printf("Deleting node :\n");
          print_node(node);
          delete_node(&node);
          break;
        }
        case DELETE_TREE: {
          printf("Deleting entire tree.\n");
          delete_tree(&root_node);
          break;
        }
        case PRINT_TREE: {
          printf("Current tree: \n");
          print_tree(root_node);
          break;
        }
      }

      /* end of switch */

    }

    /* end of  while (data_type ...  */

  }

  /* end of while(1) */

  /* never reached */

} /* main() */
