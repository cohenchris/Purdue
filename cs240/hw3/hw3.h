#ifndef HW3_H
#define HW3_H

struct complex_struct {
  double x;
  double y;
};

typedef struct complex_struct complex;

complex complex_add ( complex, complex );
complex complex_sub ( complex, complex );
complex complex_mul ( complex, complex );
complex complex_div ( complex, complex );

complex complex_neg ( complex );
complex complex_inv ( complex );
complex complex_exp ( complex );

double  complex_dot ( complex, complex );

int     mandelbrot ( complex );

#define MAX_MANDELBROT 255

#endif // HW3_H
