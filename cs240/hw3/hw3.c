/*
 * Chris Cohen, hw3.c, CS 24000, Fall 2018
 * Last updated September 20, 2018
 */

#include "hw3.h"

#include "math.h"

/*
 * This function will add the corresponding fields of two complex variables
 * together and will return a complex value that represents the sum.
 */

complex complex_add(complex complex_one, complex complex_two) {
  double x_value = 0.0;
  double y_value = 0.0;

  /*
   * x value is calculated by adding the x value of (argument 1) to the x
   * value of (argument 2). y value is calculated the same way, but with the
   * respective y values
   */

  x_value = complex_one.x + complex_two.x;
  y_value = complex_one.y + complex_two.y;

  complex final_complex = {x_value, y_value};
  return final_complex;

} /* complex_add() */

/*
 * This function will return the negative form of the given complex number
 * (argument 1).
 */

complex complex_neg(complex complex_num) {
  double x_value = 0.0;
  double y_value = 0.0;

  /*
   * x value is calculated by negating the x value of (argument 1).
   * y value is calculated the same way, but with the y value of (argument 1).
   */

  x_value = -complex_num.x;
  y_value = -complex_num.y;

  complex final_complex = {x_value, y_value};
  return final_complex;
} /* complex_neg() */

/*
 * This function will subtract the second complex number (argument 2) from
 * the first (argument 1)
 */

complex complex_sub(complex complex_one, complex complex_two) {
  double x_value = 0.0;
  double y_value = 0.0;

  /*
   * x value is calculated by subtracting the x value of (argument 1) from the
   * x value of (argument 2). y value calculated the same way, but with the
   * respective y values of each argument.
   */

  x_value = complex_one.x - complex_two.x;
  y_value = complex_one.y - complex_two.y;

  complex final_complex = {x_value, y_value};
  return final_complex;
} /* complex_sub() */

/*
 * This function will return the dot product of the complex arguments, found
 * by multiplying the x values together, the y values together, then adding
 * the two sums.
 */

double complex_dot(complex complex_one, complex complex_two) {
  double multiplied_x_values = 0.0;
  double multiplied_y_values = 0.0;

  /*
   * multiplys the x values of each argument for the multiplied_x_values
   * variable, and the y values of each argument for the multiplied_y_values
   * variable. Returns the sum of the two variables.
   */

  multiplied_x_values = complex_one.x * complex_two.x;
  multiplied_y_values = complex_one.y * complex_two.y;

  return multiplied_x_values + multiplied_y_values;
} /* complex_dot() */

/*
 * This function will calculate and return the reciprocal of the
 * argument.
 */

complex complex_inv(complex complex_num) {
  double x_value = 0.0;
  double y_value = 0.0;

  /*
   * To find the magnitude squared, square the x value of the argument, the y
   * value of the argument, and add them together.
   */

  double square_x = complex_num.x * complex_num.x;
  double square_y = complex_num.y * complex_num.y;
  double magnitude_squared = square_x + square_y;

  /*
   * x value is calculated by dividing the x value of the argument by the
   * magnitude squared. y value is calculated by dividing the y value of the
   * argument by the magnitude squared, then NEGATING it.
   */

  x_value = complex_num.x / magnitude_squared;
  y_value = -(complex_num.y / magnitude_squared);

  complex final_complex = {x_value, y_value};
  return final_complex;
} /* complex_inv() */

/*
 * This function calculates the complex product of two complex arguments
 * and returns the calculated value.
 */

complex complex_mul(complex complex_one, complex complex_two) {
  double x_value = 0.0;
  double y_value = 0.0;

  /*
   * x value found by --> (x1 * x2) - (y1 * y2)
   * y value found by --> (x1 * y2) + (y1 * x2)
   */

  x_value = (complex_one.x * complex_two.x) - (complex_one.y * complex_two.y);
  y_value = (complex_one.x * complex_two.y) + (complex_one.y * complex_two.x);

  complex final_complex = {x_value, y_value};
  return final_complex;
} /* complex_nul() */

/*
 * This function will return the complex division of the first argument
 * by the second. Uses the complex_mul and complex_inv functions.
 */

complex complex_div(complex complex_one, complex complex_two) {
  return complex_mul(complex_one, complex_inv(complex_two));
} /* complex_div() */

/*
 * This function computes the complex exponential function e^n where the
 * argument is complex.
 */

complex complex_exp(complex complex_num) {
  double x_value = 0.0;
  double y_value = 0.0;

  /*
   * x value found by --> e^x * cos(y)
   * y value found by --> e^x * sin(y)
   */

  x_value = exp(complex_num.x) * cos(complex_num.y);
  y_value = exp(complex_num.x) * sin(complex_num.y);

  complex final_complex = {x_value, y_value};
  return final_complex;
} /* complex_exp() */

/*
 * This function initializes a temp variable z and computes the operation
 * z = z^2 + (argument 1) until the square root of the dot product is
 * greater than 2.0. It then returns the amount of times that the
 * calculation needed to be made.
 */

int mandelbrot(complex complex_num) {

  #define DOT_LESS_THAN_THIS (2.0)

  complex z = {0.0, 0.0};
  int num_calculations = 0;

  /*
   * This while loop checks to see if the square root of the dot product is
   * less than 2, AND if the total number of calculations made is less than
   * MAD_MANDELBROT (255). Each time it is looped, z is set to a new value
   * (z^2 + complex_num (argument 1))m and the variable for the number of
   * calculations is increased by 1.
   */

  while ((sqrt(complex_dot(z, z)) < DOT_LESS_THAN_THIS) &&
         (num_calculations < MAX_MANDELBROT)) {
    z = complex_add(complex_mul(z, z), complex_num);
    num_calculations++;
  }

  return num_calculations;
} /* mandelbrot() */
