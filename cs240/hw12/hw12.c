/*
 * Chris Cohen, hw12.c, CS 24000, Fall 2018
 * Last updated December 2, 2018
 */

#include "object.h"
#include "graphics.h"

#include <assert.h>
#include <malloc.h>

/* Reads an object representation from a file and sets each variable in each
 * struct in the header file based on values passed in. Returns NULL if there
 * are any errors. If no errors, returns a pointer to the new object.
 */

object * read_object (const char * filename) {

#define TRUE (1)
#define ZERO (0)
#define ONE (1)
#define TWO (2)
#define THREE (3)
#define FOUR (4)
#define WORD (1000)

  /* open the file */

  FILE * f_read = NULL;
  f_read = fopen(filename, "r");
  if (f_read == NULL) {
    return NULL;
  }

  /* read the name into a string variable that is allocated an arbitrarily
   * large amount of memory that would never get used up completely. */

  char * name = malloc(WORD * sizeof(char));
  int num_read_in = fscanf(f_read, "%[^\n]\n", name);
  if ((num_read_in == EOF) ||
      (num_read_in != ONE)) {
    fclose(f_read);
    f_read = NULL;
    return NULL;
  }

  /* allocate a new object */

  object * new = malloc(sizeof(object));
  assert(new != NULL);

  /* read the number of points and polygons */

  num_read_in = fscanf(f_read, "%d %d\n", &new->num_points,
                       &new->num_polygons);
  if ((num_read_in == EOF) ||
      (num_read_in != TWO)) {
    fclose(f_read);
    f_read = NULL;
    return NULL;
  }

  /* allocate an array of points within the object using calloc so that
   * each index is set to zero.
   */

  new->points = calloc(new->num_points, sizeof(point));
  assert(new->points != NULL);

  int start_from_zero = 0;

  /* read each point into the array of points */

  for (int i = 0; i < new->num_points; i++) {
    int id = 0;
    double p1 = 0.0;
    double p2 = 0.0;
    double p3 = 0.0;

    num_read_in = fscanf(f_read, "%d %lf %lf %lf\n",
                         &id, &p1, &p2, &p3);
    if ((num_read_in == EOF) ||
        (num_read_in != FOUR)) {
      fclose(f_read);
      f_read = NULL;
      return NULL;
    }

    /* This if statement makes sure that the indices don't get messed up.
     * Some of the shapes we use start their point indices from zero, so
     * we have an integer value that functions as a boolean (since 1 is equal
     * to true in C).
     */

    if (id == ZERO) {
      start_from_zero = TRUE;
    }

    point * new_point = malloc(sizeof(point));
    new_point->arr[ZERO] = p1;
    new_point->arr[ONE] = p2;
    new_point->arr[TWO] = p3;

    new->points[i] = *new_point;

    /* Since new_point is malloced inside a for loop, we must free it every
     * iteration.
     */

    free(new_point);
    new_point = NULL;
  }

  /* allocate an array of polygons using calloc so that each index is set
   * to zero.
   */

  new->polygons = calloc(new->num_polygons, sizeof(polygon));
  assert(new->polygons != NULL);

  /* read in the name, rgb string, and number of points. The name and
   * rgb string are allocated an arbitrarily large amount of memory that
   * will never be completely filled up with what is being read in this
   * program.
   */

  for (int i = 0; i < new->num_polygons; i++) {
    char * rgb_val = malloc(WORD * sizeof(char));
    char * color = malloc(WORD * sizeof(char));

    num_read_in = fscanf(f_read, "%[^ ] %[^ ] %d ", color, rgb_val,
                         &new->polygons[i].num_points);
    if ((num_read_in == EOF) ||
        (num_read_in != THREE)) {
      fclose(f_read);
      f_read = NULL;
      return NULL;
    }

    /* allocate each polygon's point_arr value using calloc so that each index
     * is set to zero.
     */

    new->polygons[i].point_arr = calloc(new->polygons[i].num_points,
                                        sizeof(point));
    assert(new->polygons[i].point_arr != NULL);

    for (int j = 0; j < new->polygons[i].num_points; j++) {
      int index = 0;
      num_read_in = fscanf(f_read, "%d ", &index);
      if ((num_read_in == EOF) ||
          (num_read_in != ONE)) {
        fclose(f_read);
        f_read = NULL;
        return NULL;
      }

      /* This if statement refers to the one a bit further up in the code.
       * If a certain shape starts its point indices from zero, then all
       * indices are incremented by one to make sure the rest of the code
       * works properly.
       */

      if (start_from_zero == TRUE) {
        index++;
      }

      /* This statement sets each index of the point_arr based on which
       * index is read in. The point in the points array whose index
       * corresponds to the index read in will be the value set to
       * point_arr.
       */

      new->polygons[i].point_arr[j] = &new->points[index - ONE];
    }

    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

    /* convert rgb string to r, g, and b values using name_to_rgb */

    name_to_rgb(rgb_val, &r, &g, &b);

    /* set r, g, and b values in each polygon object */

    new->polygons[i].r = r;
    new->polygons[i].g = g;
    new->polygons[i].b = b;

    /* Since rgb_val and color are both malloced in this loop, we must free
     * them every iteration.
     */

    free(rgb_val);
    rgb_val = NULL;

    free(color);
    color = NULL;
  }

  /* If I malloced or calloced a variable in the function and we don't
   * need it anymore, free it and set it to NULL.
   */

  free(name);
  name = NULL;

  fclose(f_read);
  f_read = NULL;
  return new;
} /* read_object() */

/* This function deallocate argument 1 and everything that it
 * points to.
 */

void free_object (object * obj) {
  for (int i = 0; i < obj->num_polygons; i++) {
    free((obj->polygons[i]).point_arr);
    (obj->polygons[i]).point_arr = NULL;
  }

  free(obj->polygons);
  obj->polygons = NULL;

  free(obj->points);
  obj->points = NULL;

  free(obj);
  obj = NULL;
} /* free_object() */
