#include "hw4.h"

#include <stdio.h>

char *day_name[] = {
  "Mon", "Tue", "Wed", "Thu", "Fri", 0
};

char *hour_name[] = {
  " 7:00AM",
  " 8:00AM",
  " 9:00AM",
  "10:00AM",
  "11:00AM",
  "12:00PM",
  " 1:00PM",
  " 2:00PM",
  " 3:00PM",
  " 4:00PM",
  0
};

/*
 *  This function is used to print the information related to a student
 *  located at index `index` in g_student_array. That information includes
 *  name, id, classification, money amount, dorm room number, and schedule.
 */

void print_student (unsigned int index) {
  printf("Name:   %s, %s\n",
    g_student_array[index].last_name,
    g_student_array[index].first_name);
  printf("ID:     %u\n", g_student_array[index].id_number);
  if (g_student_array[index].classification == UNDERGRADUATE) {
    printf("classification: Undergraduate\n");
  }
  else {
    printf("classification: Graduate\n");
  }
  printf("money:  %.2f\n", g_student_array[index].money);
  printf("Room #: %d\n", g_student_array[index].dorm_room);

  printf("\tMon\tTue\tWed\tThu\tFri\n");
  int hour = 0;
  for (hour = HOUR_7; hour <= HOUR_16; hour++) {
    if (hour < HOUR_12) {
      printf("%2d:00AM\t", hour + 7);
    }
    else if (hour == HOUR_12) {
      printf("12:00PM\t");
    }
    else {
      printf("%2d:00PM\t", hour - 5);
    }

    int day = 0;
    for (day = MONDAY; day <= FRIDAY; day++) {
      if (g_student_array[index].schedule[day][hour] == 0) {
        printf("\t");
      }
      else {
        printf("XXXXX\t");
      }
    }
    printf("\n");
  }
} /* print_student() */

/*
 *  This function is used to run the functions implemented in hw4.c.
 *  User input is used to determine which function to run and what
 *  input is passed to it. main() prints the return value of the
 *  selected function.
 */

int main() {
  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) read_students(char *)\n"
           "2) write_students(char *,n)\n"
           "3) find_student_by_id(unsigned int)\n"
           "4) find_student_by_name(char *, char *)\n"
           "5) compute_undergraduate_percent()\n"
           "6) grant_scholarship()\n"
           "7) find_full_dorm_room()\n"
           "8) find_earlist_meeting(unsigned int, unsigned int)\n"
           "9) Print the student at array index i\n"
           "\nSelect a function: ");

    int selected_option = 333333;
    scanf("%d", &selected_option);

    if ((selected_option > 9) || (selected_option < 0)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (selected_option == 0) {
      return 0;
    }

    int int_result = 0;
    char filename[80];
    if (selected_option == 1) {
      printf("Enter the name of a file to read from: ");
      scanf("%s", filename);
      int_result = read_students( filename );
    }

    int count = 0;
    char response[80];
    if (selected_option == 2) {
      printf("Enter the name of a file to write to: ");
      scanf("%s", filename);
      printf("Enter the number of students to write: ");
      scanf("%d", &count);
      if (count > g_student_count) {
        printf("Warning: The number you entered is larger than the"
               "size of the loaded data.\n");
        printf("OK? ");
        scanf("%s", response);
        if ((response[0] != 'Y') && (response[0] != 'y')) {
          continue;
        }
      }
      int_result = write_students(filename, count);
    }

    int id_number = 0;
    if (selected_option == 3) {
      printf("Enter the ID number of a student: ");
      scanf("%d", &id_number);
      int_result = find_student_by_id(id_number);
    }

    if (selected_option == 4) {
      char first_name[80];
      char last_name[80];
      printf("Enter the last name of a student:  ");
      scanf("%s", last_name);
      printf("Enter the first name of a student: ");
      scanf("%s", first_name);
      int_result = find_student_by_name(first_name, last_name);
    }

    float float_result = 0.0;
    if (selected_option == 5) {
      float_result = compute_undergraduate_percent();
    }

    if (selected_option == 6) {
      int_result = grant_scholarship();
    }

    if (selected_option == 7) {
      int_result = find_full_dorm_room();
    }

    int id_number2 = 0;
    if (selected_option == 8) {
      printf("Enter the ID of the first student:  ");
      scanf("%d", &id_number);
      printf("Enter the ID of the second student: ");
      scanf("%d", &id_number2);
      int_result = find_earliest_meeting(id_number, id_number2);
    }

    if (selected_option == 9) {
      printf("Enter the array index of a student to print: ");
      scanf("%d", &count);

      if (count > g_student_count) {
        printf("Warning: The number you entered is larger than the"
               " size of the loaded data.\n");
        printf("OK? ");
        scanf("%s", response);
        if ((response[0] != 'Y') && (response[0] != 'y')) {
          continue;
        }
      }
      print_student(count);
    }

    switch (selected_option) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 6:
      case 7:
      case 8:
        if (int_result == NON_READABLE_FILE) {
          printf("Function returned NON_READABLE_FILE.\n");
        }
        else if (int_result == NON_WRITABLE_FILE) {
          printf("Function returned NON_WRITABLE_FILE.\n");
        }
        else if (int_result == READ_BAD_RECORD) {
          printf("Function returned READ_BAD_RECORD.\n");
        }
        else if (int_result == NOT_FOUND) {
          printf("Function returned NOT_FOUND.\n");
        }
        else if (int_result == NOT_SCHEDULABLE) {
          printf("Function returned NOT_SCHEDULABLE.\n");
        }
        else {
          if (selected_option != 8) {
            printf("Function returned %d.\n", int_result);
          }
          else {
            printf("Function returned %s, %s", day_name[int_result / 100],
              hour_name[(int_result - 7) % 100]);
          }
        }
        break;

      case 5:
        printf("Function returned %.2f.\n", float_result);
        break;

      default:
        break;
    }

  }

  /* not reached. */

  return 0;
} /* main() */

