#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL/SDL.h>

/*
 *=============================================================================
 * Types for graphics support code.
 *=============================================================================
 */

typedef struct point2d {
  int x;
  int y;
} point2d;

/*
 *=============================================================================
 * Prototypes for graphics support code.
 *=============================================================================
 */

void sdl_lock(SDL_Surface *screen);

void sdl_unlock(SDL_Surface *screen);

void draw_pixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b);

void draw_line(SDL_Surface *screen, int x1, int y1, int x2, int y2,
              Uint8 r, Uint8 g, Uint8 b);

void draw_polygon(SDL_Surface *screen,
                 point2d *points,
                 int num_points,
                 Uint8 r, Uint8 g, Uint8 b);

void draw_shade_background(SDL_Surface *screen);

void name_to_rgb(const char *name, Uint8 *r, Uint8 *g, Uint8 *b);

void fill_polygon (SDL_Surface *screen, point2d * pts, int cnt,
                  Uint8 r, Uint8 g, Uint8 b);

#endif // GRAPHICS_H

