#include "graphics.h"

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>

#define ABS(x) ((x) > 0 ? (x) : -(x))
#define MAX_EDGES (10000)
#define MAX_RGB_VALUE (255.0)

/*
 *===========================================================================
 * These functions lock and unlock the drawing surface.
 * On many systems, the drawing surface must be locked in order to
 * be able to modify it.
 *===========================================================================
 */

/*
 *  Lock the screen to allow the screen to be drawn on.
 */

void sdl_lock(SDL_Surface *screen) {
  if (SDL_MUSTLOCK(screen)) {
    if (SDL_LockSurface(screen) < 0) {
      return;
    }
  }
} /* sdl_lock() */

/*
 *  Unlock the screen.
 */

void sdl_unlock(SDL_Surface *screen) {
  if (SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen);
  }
} /* sdl_unlock() */

/*
 * Draw a pixel on the screen at point x, y.
 */

void draw_pixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
  Uint32 color = SDL_MapRGB(screen->format, r, g, b);

  /*  Clipping. */

  if ((x < 0) || (x >= screen->w) || (y < 0) || (y >= screen->h)) {
    return;
  }

  switch (screen->format->BytesPerPixel) {
    case 1: { /* Assuming 8-bpp */
      Uint8 *bufp = NULL;

      bufp = (Uint8 *) screen->pixels + y * screen->pitch + x;
      *bufp = color;
      break;
    }

    case 2: { /* Probably 15-bpp or 16-bpp */
      Uint16 *bufp = NULL;

      bufp = (Uint16 *)screen->pixels + y*screen->pitch / 2 + x;
      *bufp = color;
      break;
    }

    case 3: { /* Slow 24-bpp mode, usually not used */
      Uint8 *bufp = NULL;

      bufp = (Uint8 *)screen->pixels + y * screen->pitch + x * 3;
      if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
        bufp[0] = color;
        bufp[1] = color >> 8;
        bufp[2] = color >> 16;
      }
      else {
        bufp[2] = color;
        bufp[1] = color >> 8;
        bufp[0] = color >> 16;
      }
      break;
    }

    case 4: { /* Probably 32-bpp */
      Uint32 *bufp = NULL;

      bufp = (Uint32 *)screen->pixels + y * screen->pitch / 4 + x;
      *bufp = color;
      break;
    }
  }
} /* draw_pixel() */

/*
 *  Implementation of Bresenham's line algorithm to draw a straight line
 *  between points x1, y1 and x2, y2.
 */

void bresenham(SDL_Surface *screen, int invert, int x1, int y1, int x2, int y2,
               Uint8 r, Uint8 g, Uint8 b) {
  int x = 0;
  int y = 0;
  int dx = 0;
  int dy = 0;
  int d = 0;
  int incr1 = 0;
  int incr2 = 0;
  int xend = 0;
  int incy = 1;

  /* Put the segment into the right quadrant for Bresenham's algorithm. */

  if (((x2 > x1) && (y2 < y1)) || ((x2 < x1) && (y2 > y1))) {
    incy = -1;
  }

  /* Basically Bresenham's algorithm. */

  dx = ABS(x2-x1);
  dy = ABS(y2-y1);
  d = 2 * dy - dx;
  incr1 = 2 * dy;
  incr2 = 2 * (dy - dx);

  if (x1 > x2) {
    x = x2;
    y = y2;
    xend = x1;
  }
  else {
    x = x1;
    y = y1;
    xend = x2;
  }
  if (invert == 1) {
    draw_pixel(screen, y, x, r, g, b);
  }
  else {
    draw_pixel(screen, x, y, r, g, b);
  }

  while (x < xend) {
    x = x + 1;
    if (d < 0) {
      d = d + incr1;
    }
    else {
      y = y + incy;
      d = d + incr2;
    }
    if (invert == 1) {
      draw_pixel(screen, y, x, r, g, b);
    }
    else {
      draw_pixel(screen, x, y, r, g, b);
    }
  }
} /* bresenham() */

/*
 *  Use Bresenham's algorithm to draw a line between two points.
 */

void draw_line(SDL_Surface *screen, int x1, int y1, int x2, int y2,
               Uint8 r, Uint8 g, Uint8 b) {
  if ((ABS(y2 - y1)) > (ABS(x2 - x1))) {
    bresenham(screen, 1, y1, x1, y2, x2, r, g, b);
  }
  else {
    bresenham(screen, 0, x1, y1, x2, y2, r, g, b);
  }
} /* draw_line() */

/*
 *  Draw a shape on the screen defined by points.
 */

void draw_polygon(SDL_Surface *screen, point2d *points, int num_points,
                  Uint8 r, Uint8 g, Uint8 b) {
  for (int i = 1; i < num_points; i++) {
    draw_line(screen, points[i - 1].x, points[i - 1].y, points[i].x,
              points[i].y, r, g, b);
  }
  draw_line(screen, points[0].x, points[0].y, points[num_points - 1].x,
            points[num_points - 1].y, r, g, b);
} /* draw_polygon() */

/*
 *  Draw shade over entire background of screen.
 */

void draw_shade_background(SDL_Surface *screen) {
  for (int x = 0; x < screen->w; x++) {
    for (int y = 0; y < screen->h; y++) {
      int n = MAX_RGB_VALUE * (x + y) / (screen->w + screen->h);
      draw_pixel(screen, x, y, n, n, n);
    }
  }
} /* draw_shade_background() */

/*
 *  Translate hex code to rgb value.
 */

void name_to_rgb(const char *name, Uint8 *r, Uint8 *g, Uint8 *b) {
  char color[3] = "\0";
  int x = 0;

  if (name[0] != '#') {
    return;
  }

  color[0] = name[1];
  color[1] = name[2];
  color[2] = 0;
  sscanf(color, "%2x", &x);
  *r = x;

  color[0] = name[3];
  color[1] = name[4];
  color[2] = 0;
  sscanf(color, "%2x", &x);
  *g = x;

  color[0] = name[5];
  color[1] = name[6];
  color[2] = 0;
  sscanf(color, "%2x", &x);
  *b = x;
} /* name_to_rgb() */



/*
 *===========================================================================
 * Polygon filling is rather difficult.
 * I stole most of this code from
 * Hearn & Baker's _Computer Graphics_, 2nd Edition.
 * It doesn't appear to do a very rational job with clipping.
 *===========================================================================
 */

typedef struct tedge {
  int yupper;
  float xintersect;
  float dxperscan;
  struct tedge *next;
} edge;

edge g_edge_table[MAX_EDGES];
edge *g_edge_table_ptr;

/*
 *  If g_edge_table is less than the max number of edges, increment
 *  g_edge_table_ptr.
 */

edge *edge_alloc() {
  if (g_edge_table_ptr >= &g_edge_table[MAX_EDGES]) {
    printf("g_edge_table overflow.\n");
  }
  return g_edge_table_ptr++;
} /* edge_alloc() */

/*
 *  Inserts edge into list in order of increasing xintersect field.
 */

void insert_edge(edge *list, edge *e) {
  edge *edge_one = NULL;
  edge *edge_two = list;

  edge_one = edge_two->next;
  while (edge_one != NULL) {
    if (edge_one->next == edge_one) { /* JEFF: why does this happen? */
      edge_one->next = NULL;
    }
    if (e->xintersect < edge_one->xintersect) {
      edge_one = NULL;
    }
    else {
      edge_two = edge_one;
      edge_one = edge_one->next;
    }
  }
  e->next = edge_two->next;
  edge_two->next = e;
} /* insert_edge() */

/*
 *  For an index, return y-coordinate of next nonhorizontal line.
 */

int y_next(int k, int cnt, point2d *pts) {
  int j = 0;

  if ((k + 1) > (cnt - 1)) {
    j = 0;
  }
  else {
    j = k + 1;
  }

  while (pts[k].y == pts[j].y) {
    if ((j + 1) > (cnt - 1)) {
      j = 0;
    }
    else {
      j++;
    }
  }

  return (pts[j].y);
} /* y_next() */

/*  Store lower-y coordinate and inverse slope for each edge.  Adjust
 *  and store upper-y coordinate for edges that are the lower member
 *  of a monotically increasing or decreasing pair of edges.
 */

void make_edge_rec(point2d lower, point2d upper, int ycomp,
                   edge *e, edge *edges[]) {

  if (lower.y >= 480 || lower.y < 0) { /* Outside the window */
    return;
  }

  e->dxperscan = (float) (upper.x - lower.x) / (upper.y - lower.y);
  e->xintersect = lower.x;
  if (upper.y < ycomp) {
    e->yupper = upper.y - 1;
  }
  else {
    e->yupper = upper.y;
  }
  insert_edge(edges[lower.y], e);
} /* make_edge_rec() */

/*
 *  Build edges from point to point in pts.
 */

void build_edge_list(int cnt, point2d *pts, edge *edges[]) {
  edge *e = NULL;
  point2d v1 = { 0 };
  point2d v2 = { 0 };
  int y_prev = pts[cnt - 2].y;

  v1.x = pts[cnt - 1].x;
  v1.y = pts[cnt - 1].y;

  for (int i = 0; i < cnt; i++) {
    v2 = pts[i];
    if (v1.y != v2.y) {                /* nonhorizontal line */
      e = edge_alloc();
      if (v1.y < v2.y) {               /* up-going edge      */
        make_edge_rec(v1, v2, y_next(i, cnt, pts), e, edges);
      }
      else {                           /* down-going edge    */
        make_edge_rec(v2, v1, y_prev, e, edges);
      }
    }
    y_prev = v1.y;
    v1 = v2;
  }
} /* build_edge_list() */

/*
 *  Populate active list of edges from edges.
 */

void build_active_list(int scan, edge *active, edge *edges[]) {
  edge *edge_one = edges[scan]->next;
  while (edge_one) {
    edge *edge_two = edge_one->next;
    insert_edge(active, edge_one);
    edge_one = edge_two;
  }
} /* build_active_list() */

/*
 *  Fill active edges on the screen.
 */

void fill_scan(SDL_Surface *screen, int scan, edge *active,
               Uint8 r, Uint8 g, Uint8 b) {
  edge *edge_one = active->next;
  while (edge_one) {
    if (edge_one->next == NULL) {
      edge_one = edge_one->next;
    }
    else {
      edge *edge_two = edge_one->next;
      for (int i = edge_one->xintersect; i < edge_two->xintersect; i++) {
        draw_pixel(screen, i, scan, r, g, b);
      }
      edge_one = edge_two->next;
    }
  }
} /* fill_scan() */

/*
 *  Delete edge. Used to update the active list.
 */

void delete_after(edge *edge_one) {
  edge *edge_two = edge_one->next;
  edge_one->next = edge_two->next;
} /* delete_after() */

/*
 * Delete completed edges. Update 'xintersect' field for others.
 */

void update_active_list(int scan, edge *active) {
  edge *edge_one = active;
  edge *edge_two = active->next;

  while (edge_two) {
    if (scan >= edge_two->yupper) {
      edge_two = edge_two->next;
      delete_after(edge_one);
    }
    else {
      edge_two->xintersect = edge_two->xintersect + edge_two->dxperscan;
      edge_one = edge_two;
      edge_two = edge_two->next;
    }
  }
} /* update_active_list() */

/*
 * Insert edges in active list.
 */

void resort_active_list(edge *active) {
  edge *edge_one = NULL;
  edge *edge_two = active->next;

  active->next = NULL;
  while (edge_two) {
    edge_one = edge_two->next;
    insert_edge(active, edge_two);
    edge_two = edge_one;
  }
} /* resort_active_list() */

/*
 *  Fill polygon defined by points in pts on screen.
 */

void fill_polygon(SDL_Surface *screen, point2d *pts, int cnt,
                  Uint8 r, Uint8 g, Uint8 b) {
  edge *edges[screen->h];
  edge *active = NULL;

  g_edge_table_ptr = g_edge_table;
  for (int i = 0; i < screen->h; i++) {
    edges[i] = edge_alloc();
    edges[i]->next = NULL;
  }
  build_edge_list(cnt, pts, edges);
  active = edge_alloc();
  active->next = NULL;

  for (int scan = 0; scan < screen->h; scan++) {
    build_active_list(scan, active, edges);
    if (active->next) {
      fill_scan(screen, scan, active, r, g, b);
      update_active_list(scan, active);
      resort_active_list(active);
    }
  }
} /* fill_polygon() */
