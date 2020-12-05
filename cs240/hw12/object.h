#ifndef OBJECT_H
#define OBJECT_H

#include <SDL/SDL.h>

/*
 *=============================================================================
 * Types for object manipulation code...
 *=============================================================================
 */

typedef struct vector {
  double arr[4];
} vector;

typedef vector point;


typedef struct matrix {
  double arr[4][4];
} matrix;

typedef struct polygon {
  int num_points;
  point **point_arr;
  unsigned char r;
  unsigned char g;
  unsigned char b;
} polygon;

typedef struct object {
  point   location;
  matrix  orientation;

  int num_polygons;
  int num_points;
  polygon *polygons;
  point   *points;
} object;


/*
 *=============================================================================
 * Prototypes for object manipulation code.
 *=============================================================================
 */

matrix mult(matrix a, matrix b);
vector vmult(vector v, matrix m);
vector multv(matrix m, vector v);
vector vadd(vector a, vector b);
vector vsub(vector a, vector b);
vector vcross3d(vector a, vector b);
matrix identity();
matrix zero();
matrix rotate_x(double d);
matrix rotate_y(double d);
matrix rotate_z(double d);
matrix magnify(double x, double y, double z);
vector move_to(double x, double y, double z);
matrix orientation(double pitch, double yaw, double heading);
void render(SDL_Surface *screen, object *obj, matrix trans);

object *read_object(const char *filename);
void free_object(object *obj);

#endif // OBJECT_H
