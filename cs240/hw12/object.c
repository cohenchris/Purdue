#include <math.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "object.h"
#include "graphics.h"

#define LENGTH (4)

/*
 *===========================================================================
 * Object manipulation stuff.
 *===========================================================================
 */

/*
 *  Multiply two 4x4 matrices.
 */

matrix mult(matrix a, matrix b) {
  matrix r = { 0 };
  for (int x = 0; x < LENGTH; x++) {
    for (int y = 0; y < LENGTH; y++) {
      r.arr[x][y] = 0;
      for (int n = 0; n < LENGTH; n++) {
        r.arr[x][y] += a.arr[x][n] * b.arr[n][y];
      }
    }
  }
  return r;
} /* mult() */

/*
 *  Multiply a Row vector by a matrix.
 */

vector vmult(vector v, matrix m) {
  vector r = { 0 };

  for (int x = 0; x < LENGTH; x++) {
    r.arr[x] = 0;
    for (int n = 0; n < LENGTH; n++) {
      r.arr[x] += v.arr[n] * m.arr[x][n];
    }
  }

  return r;
} /* vmult() */

/*
 *  Multiply a matrix by a Column vector.
 */

vector multv(matrix m, vector v) {
  vector r = { 0 };

  for (int y = 0; y < LENGTH; y++) {
    r.arr[y] = 0;
    for (int n = 0; n < LENGTH; n++) {
      r.arr[y] += m.arr[n][y] * v.arr[n];
    }
  }

  return r;
} /* multv() */

/*
 *  Add two vectors.
 */

vector vadd(vector a, vector b) {
  vector r = { 0 };

  for (int n = 0; n < LENGTH; n++) {
    r.arr[n] = a.arr[n] + b.arr[n];
  }

  return r;
} /* vadd() */

/*
 *  Subtract two vectors.
 */

vector vsub(vector a, vector b) {
  vector r = { 0 };

  for (int n = 0; n < LENGTH; n++) {
    r.arr[n] = a.arr[n] - b.arr[n];
  }

  return r;
} /* vsub() */

/*
 *  Compute the 3D cross-product.
 */

vector vcross3d(vector a, vector b) {
  vector r = { 0 };

  r.arr[0] = a.arr[1] * b.arr[2]  -  a.arr[2] * b.arr[1];
  r.arr[1] = -a.arr[0] * b.arr[2] -  a.arr[2] * b.arr[0];
  r.arr[2] = a.arr[0] * b.arr[1]  -  a.arr[1] * b.arr[0];
  r.arr[3] = 1;
  return r;
} /* vcross3d() */

/*
 *  Form a 4x4 identity matrix.
 */

matrix identity(void) {
  matrix r =
    { { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, {0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
  return r;
} /* identity() */

/*
 *  Form a 4x4 zero matrix.
 */

matrix zero(void) {
  matrix r =
    { { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } } };
  return r;
} /* zero() */

/*
 *  Form a transformation matrix that will rotate by d Degrees around
 *  the X axis.
 */

matrix rotate_x(double d) {
  matrix r = identity();

  d = d * M_PI / 180.0;
  r.arr[1][1] = cos(d);
  r.arr[1][2] = sin(d);
  r.arr[2][1] = -sin(d);
  r.arr[2][2] = cos(d);
  return r;
} /* rotate_x() */

/*
 *  Form a transformation matrix that will rotate by d Degrees around
 *  the Y axis.
 */

matrix rotate_y(double d) {
  matrix r = identity();

  d = d * M_PI / 180.0;
  r.arr[0][0] = cos(d);
  r.arr[0][2] = -sin(d);
  r.arr[2][0] = sin(d);
  r.arr[2][2] = cos(d);
  return r;
} /* rotate_y() */

/*
 *  Form a transformation matrix that will rotate by d Degrees around
 *  the Z axis.
 */

matrix rotate_z(double d) {
  matrix r = identity();

  d = d * M_PI / 180.0;
  r.arr[0][0] = cos(d);
  r.arr[0][1] = sin(d);
  r.arr[1][0] = -sin(d);
  r.arr[1][1] = cos(d);
  return r;
} /* rotate_z() */

/*
 *  Form a transformation matrix that will magnify by x y and z dimensions.
 */

matrix magnify(double x, double y, double z) {
  matrix r = identity();
  r.arr[0][0] = x;
  r.arr[1][1] = x;
  r.arr[2][2] = x;
  return r;
} /* magnify() */

/*
 *  Form a vector.
 */

vector move_to(double x, double y, double z) {
  vector r = { {x, y, z, 0} };
  return r;
} /* move_to() */

/*
 *  Orient in three degrees of freedom.
 */

matrix orientation(double pitch, double yaw, double heading) {
  matrix r = { 0 };
  matrix x = { 0 };
  matrix y = { 0 };
  matrix z = { 0 };

  /* Combine transforms in order... X, Y then Z axis. */

  x = rotate_x(yaw);
  y = rotate_y(pitch);
  z = rotate_z(heading);
  r = mult(mult(x, y), z);

  return r;
} /* orientation() */

/*
 *  Display the ojbect obj on the screen.
 */

void render(SDL_Surface *screen, object *obj, matrix trans) {
  for (int i = 0; i < obj->num_polygons; i++) {
    point2d arr[32] = { 0 };
    vector v1 = { 0 };
    vector v2 = { 0 };
    vector v3 = { 0 };

    /* Check the cross-product of two adjacent segments of the polygon */
    /* to determine whether the face is facing in or out of the object. */

    v1 = vsub(multv(trans, *(obj->polygons[i].point_arr[1])),
              multv(trans, *(obj->polygons[i].point_arr[0])) );
    v2 = vsub(multv(trans, *(obj->polygons[i].point_arr[1])),
              multv(trans, *(obj->polygons[i].point_arr[2])) );
    v3 = vcross3d(v1, v2);

    /* If the face is drawn clockwise, render it. */

    if (v3.arr[2] > 0) {
      for (int j = 0; j < obj->polygons[i].num_points; j++) {
        point p = multv(trans, *(obj->polygons[i].point_arr[j]));
        arr[j].x = p.arr[0] + obj->location.arr[0];
        arr[j].y = p.arr[1] + obj->location.arr[1];
      }
      fill_polygon(screen, arr, obj->polygons[i].num_points,
                   obj->polygons[i].r,
                   obj->polygons[i].g,
                   obj->polygons[i].b);
      draw_polygon(screen, arr, obj->polygons[i].num_points, 0, 0, 0);
    }
  }
} /* render() */
