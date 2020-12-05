#include "hw2.h"

#include <stdio.h>

/*  This function prints the integer value val. If val corresponds to an
 *  error code, print the error code.
 */

void print_return_value(int val) {
  printf("The value returned by the function was: %d ", val);
  switch (val) {
    case FILE_READ_ERROR:
      printf("(FILE_READ_ERROR)\n");
      break;
    case FILE_WRITE_ERROR:
      printf("(FILE_WRITE_ERROR)\n");
      break;
    case RECORD_ERROR:
      printf("(RECORD_ERROR)\n");
      break;
    case OUT_OF_BOUNDS:
      printf("(OUT_OF_BOUNDS)\n");
      break;
    case NO_SUCH_PRODUCT:
      printf("(NO_SUCH_PRODUCT)\n");
      break;
    default:
      printf("\n");
      break;
  }

  return;
} /* print_return_value() */

/*  This function prints the name, price, and availability of the product
 *  represented by the integer val. If val is does not correspond to a product,
 *  print an appropriate error code.
 */

void print_product(int val) {
  if (val < g_product_count) {
    printf("The product offset was %d.\n", val);
    printf("That corresponds to the following product:\n");
    printf("%s,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
           g_product_name[val],
           g_unit_price[val],
           g_availability[val][0],
           g_availability[val][1],
           g_availability[val][2],
           g_availability[val][3],
           g_availability[val][4],
           g_availability[val][5],
           g_availability[val][6],
           g_availability[val][7],
           g_availability[val][8],
           g_availability[val][9]);
  }
  else {
    print_return_value(val);
  }
} /* print_product() */

/*  This function is used to run the functions implemented in hw2.c.
 *  User input is used to determine which function to run and what
 *  input is passed to it. Upon completion, the funciton returns zero.
 */

int main() {
  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) input_tables()\n"
           "2) unit_price()\n"
           "3) net_worth()\n"
           "4) max_net_product()\n"
           "5) max_prod_availability()\n"
           "6) max_store_availability()\n"
           "7) output_tables()\n"
           "\nSelect a function: ");

    int num = 333333;
    int status = scanf("%d", &num);
    if (status == 0) {
      continue;
    }

    if (num > 7) {
      printf("Invalid selection.\n");
      continue;
    }

    if (num == 0) {
      return 0;
    }

    char file[1000];
    if ((num == 1) || (num == 7)) {
      printf("Enter the file name: ");
      scanf("%s", file);
    }

    char product[1000];
    if ((num == 2) || (num == 5)) {
      printf("Enter the product name: ");
      scanf("\n%[^\n]", product);
      printf("Product is '%s'\n", product);
    }

    int val = 0;
    if ((num == 3) || (num == 4) || (num == 6)) {
      printf("Enter the store number: ");
      scanf("%d", &val);
    }

    int store_one = 0;
    int store_two = 0;
    if (num == 7) {
      printf("Enter the starting store number: ");
      scanf("%d", &store_one);
      printf("Enter the ending store number:   ");
      scanf("%d", &store_two);
    }

    float float_val = 0.0;
    switch (num) {
      case 1:
        val = input_tables(file);
        print_return_value(val);
        break;
      case 2:
        float_val = unit_price(product);
        printf("Unit price was %3f\n", float_val);
        break;
      case 3:
        float_val = net_worth(val);
        printf("Net worth was %3f\n", float_val);
        break;
      case 4:
        float_val = max_net_product(val);
        printf("Maximum net product value was %3f\n", float_val);
        break;
      case 5:
        val = max_prod_availability(product);
        print_return_value(val);
        break;
      case 6:
        val = max_store_availability(val);
        print_product(val);
        break;
      case 7:
        val = output_tables(file, store_one, store_two);
        print_return_value(val);
        break;
      default:
        break;
    }
  }

  /* not reached. */

  return 0;
} /* main() */
