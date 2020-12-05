#include "hw3.h"

#include <stdio.h>

/*
 *  This function is used to run the functions implemented in hw3.c.
 *  User input is used to determine which function to run and what
 *  input is passed to it. main() prints the return value of the
 *  selected function.
 */

int main() {
  while (1) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) complex_neg(complex)\n"
           "2) complex_inv(complex)\n"
           "3) complex_exp(complex)\n"
           "4) mandelbrot(complex)\n"
           "5) complex_add(complex, complex)\n"
           "6) complex_sub(complex, complex)\n"
           "7) complex_mul(complex, complex)\n"
           "8) complex_div(complex, complex)\n"
           "9) complex_dot(complex, complex)\n"
           "\nSelect a function: ");

    int num = 333333;
    scanf("%d", &num);

    if ((num > 9) || (num < 0)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (num == 0) {
      return 0;
    }

    printf("Enter a complex number in the form x,y:       ");

    complex comp_one = {0.0, 0.0};
    complex comp_two = {0.0, 0.0};
    scanf("%lf,%lf", &(comp_one.x), &(comp_one.y));

    if (num > 4) {
      printf("Enter another complex number in the form x,y: ");
      scanf("%lf,%lf", &(comp_two.x), &(comp_two.y));
    }

    complex result = {0.0, 0.0};
    double double_result = 0.0;
    int int_result = 0;
    switch (num) {
      case 1:
        result = complex_neg(comp_one);
        break;
      case 2:
        result = complex_inv(comp_one);
        break;
      case 3:
        result = complex_exp(comp_one);
        break;
      case 4:
        int_result = mandelbrot(comp_one);
        break;
      case 5:
        result = complex_add(comp_one, comp_two);
        break;
      case 6:
        result = complex_sub(comp_one, comp_two);
        break;
      case 7:
        result = complex_mul(comp_one, comp_two);
        break;
      case 8:
        result = complex_div(comp_one, comp_two);
        break;
      case 9:
        double_result = complex_dot(comp_one, comp_two);
        break;
    }

    if (num == 4) {
      printf("The value returned was %d\n", int_result);
    }
    else if (num == 9) {
      printf("The value returned was %f\n", double_result);
    }
    else {
      printf("The value returned was (%f,%f)\n", result.x, result.y);
    }
  }

  /* not reached. */

  return 0;
} /* main() */

