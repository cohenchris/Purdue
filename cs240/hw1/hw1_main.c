#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw1.h"

/*  This function prints out all of the characters in the file named
 *  filename. It returns FILE_READ_ERR if it cannot read the specified
 *  file. Otherwise, it returns OK to indicate that the funciton was
 *  successful.
 */

int print_out(char *filename) {
  FILE *file_ptr = NULL;
  file_ptr = fopen(filename, "r");
  if (file_ptr == NULL) {
    return (FILE_READ_ERR);
  }

  int character = '\0';
  while ((character = fgetc(file_ptr)) != EOF) {
    putchar(character);
  }

  fclose(file_ptr);
  file_ptr = NULL;
  return (OK);
} /* print_out() */

/*  This function is used to run the functions implemented in hw1.c.
 *  User input is used to determine which function to run what input
 *  is passed to it. Upon completion, the function returns zero.
 */

int main(int argc, char **argv) {
  while (1) {
    printf("\n\n\n\n");
    printf("Test Menu\n");
    printf("1. Get Section Average\n");
    printf("2. Obtain Top Score Out of All Sections\n");
    printf("3. Create List of Students Failing\n");
    printf("4. Create Final Grade Report\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    int response = 0;
    scanf("%d", &response);
    getchar();

    int section = 0;
    float total = 0.0;
    char input_filename[MAXFILELEN] = "";
    char result_filename[MAXFILELEN] = "";
    switch (response) {
      case 1:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
        printf("Enter section number: ");
	      scanf("%d", &section);
        total = get_section_average(input_filename, section);
        if (total >= 0) {
		      printf("Section average in %s for section %d is %.2f\n",
            input_filename, section, total);
        }
        else {
	  	    printf("Error! (code %d)\n", (int) total);
        }
        break;

      case 2:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
        printf("\nget_top_score(%s) returned %d\n", input_filename,
          get_top_score(input_filename));
        break;

      case 3:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
	      printf("Enter output file name: ");
	      scanf("%s", result_filename);
        printf("Enter cutoff number: ");
        scanf("%d", &section);
	      printf("create_fail_list(%s, %s, %d) returned %d\n", input_filename,
          result_filename, section, create_fail_list(input_filename,
          result_filename, section));
	      printf("File %s contains:\n", result_filename);
	      print_out(result_filename);	
	      break;

      case 4:
        printf("Enter input file name: ");
        scanf("%s", input_filename);
        printf("Enter output file name: ");
        scanf("%s", result_filename);
        printf("Enter section number: ");
        scanf("%d", &section);
        printf("create_grade_report(%s, %s, %d) returned %d\n", input_filename,
          result_filename, section, create_grade_report(input_filename,
          result_filename, section));
	      printf("File %s contains:\n", result_filename);
	      print_out(result_filename);
	      break;

      case 5:
        printf("GooD bYe :^) \n\n");
        return (0);

      default: 
        printf("Invalid choice!\n");
        break;

    }
  }

  return (0);
} /* main() */
