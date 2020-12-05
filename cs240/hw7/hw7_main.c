#include "hw7.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define QUIT (0)
#define ENTER_STRING_ONE (1)
#define ENTER_STRING_TWO (2)
#define APPEND_CHAR (3)
#define PREPEND_CHAR (4)
#define STRING_TO_LIST (5)
#define APPEND_LISTS (6)
#define LIST_TO_STRING (7)
#define COPY_STRING (8)
#define CONCAT_STRINGS (9)
#define UPPERCASE_FREE_LISTS 'A'
#define LOWERCASE_FREE_LISTS 'a'

/*
 *  This function prints the value of each element of the list passed
 *  into it.
 */

void display_list(struct node *list) {
  printf("Elements of the list are :\n");

  while (list != NULL) {
    printf("%c ", list->val);
    list = list->next;
  }
  printf("<END>\n");
} /* display_list() */

/*
 *  This function is used to run the functions in hw7.c. User input is used
 *  to determine which function to run and what input is passed to it.
 *  main() prints information related to running the chosen function.
 */

int main() {
  // Disable stdlib I/O buffering

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  char string_one[256] = "\0";
  char string_two[256] = "\0";
  struct node *list_one = NULL;
  struct node *list_two = NULL;

  while (1) {
    int is_empty_one = (string_one[0] == '\0') ? 1 : 0;
    int is_empty_two = (string_two[0] == '\0') ? 1 : 0;
    char user_choice = '\0';

    printf("\n\n"
      "0. Quit\n"
      "1. %s (%s)\n"
      "2. %s (%s)\n"
      "3. append_char(list_one, character)\n"
      "4. prepend_char(list_two, character)\n"
      "5. string_to_list(string_one, reverse_character)\n"
      "6. append_lists(list_one, list_two)\n"
      "7. list_to_string(list_one)\n"
      "8. copy_string(string_one, reverse_character)\n"
      "9. concat_strings(string_two, string_two)\n"
      "A. Free Lists & Reset\n",
      is_empty_one ? "Initialize string_one" : "Change string_one",
      is_empty_one ? "<EMPTY>" : string_one,
      is_empty_two ? "Initialize string_two" : "Change string_two",
      is_empty_two ? "<EMPTY>" : string_two);

    printf("Enter user_choice: ");
    scanf("%c", &user_choice);
    getchar();
    user_choice -= 48;

    if ((user_choice < QUIT) ||
      ((user_choice > CONCAT_STRINGS) &&
      (user_choice != UPPERCASE_FREE_LISTS - 48) &&
      (user_choice != LOWERCASE_FREE_LISTS - 48))) {
      printf("Invalid selection!\n");
      continue;
    }

    if (user_choice == QUIT) {
      return 0;
    }

    if (user_choice == ENTER_STRING_ONE) {
      printf("Enter string_one: ");
      scanf("%[^\n]", string_one);
      getchar();
    }

    if (user_choice == ENTER_STRING_TWO) {
      printf("Enter string_two: ");
      scanf("%[^\n]", string_two);
      getchar();
    }

    char character = '\0';
    if ((user_choice == APPEND_CHAR) || (user_choice == PREPEND_CHAR)) {
      printf("Enter character: ");
      scanf("%c", &character);
      getchar();
    }

    int reverse = NO;
    int reverse_or_duplicate = DUPLICATE;
    if ((user_choice == COPY_STRING) || (user_choice == STRING_TO_LIST)) {
      char reverse_character = '\0';
      printf("reverse_character (y/n)? ");
      scanf("%c", &reverse_character);
      getchar();
      reverse = (reverse_character == 'y' || reverse_character == 'Y') ?
        YES : NO;
      reverse_or_duplicate  = (reverse == YES) ? REVERSE : DUPLICATE;
    }

    char *character_output = NULL;
    switch (user_choice) {
      case QUIT:
        return (0);
        break;

      case ENTER_STRING_ONE:
        break;

      case ENTER_STRING_TWO:
        break;

      case APPEND_CHAR:
        list_one = append_char(list_one, character);
        display_list(list_one);
        break;

      case PREPEND_CHAR:
        list_two = prepend_char(list_two, character);
        display_list(list_two);
        break;

      case STRING_TO_LIST:
        if (list_one != NULL) {
          printf("Freeing list 1\n");
          free_list(list_one);
          list_one = NULL;
        }
        list_one = string_to_list(string_one, reverse);
        printf("string_one is %s\n", string_one);
        display_list(list_one);
        break;

      case APPEND_LISTS:
        append_lists(list_one, list_two);
        display_list(list_one);
        break;

      case LIST_TO_STRING:
        character_output = list_to_string(list_one);
        printf("Strig returned is:\n%s\n", character_output);
        free(character_output);
        character_output = NULL;
        break;

      case COPY_STRING:
        character_output = copy_string(string_one, reverse_or_duplicate);
        printf("string_one is %s\n", string_one);
        printf("Strig returned is:\n%s\n", character_output);
        free(character_output);
        character_output = NULL;
        break;

      case CONCAT_STRINGS:
        character_output = concat_strings(string_one, string_two);
        printf("string_one is %s\n", string_one);
        printf("string_two is %s\n", string_two);
        printf("Strig returned is:\n%s\n", character_output);
        free(character_output);
        character_output = NULL;
        break;

      default:
        printf("Freeing list_one...\n");
        free_list(list_one);
        printf("Freeing list_two...\n");
        free_list(list_two);
        list_one = NULL;
        list_two = NULL;
        string_one[0] = '\0';
        string_two[0] = '\0';
        break;
    }
  }
  return 0;
} /* main() */

