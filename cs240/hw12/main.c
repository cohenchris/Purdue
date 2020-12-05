#include <math.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_endian.h>

#include "object.h"
#include "graphics.h"

/*
 *===========================================================================
 * main
 *===========================================================================
 */

object *g_obj;

/*
 *  Quit SDL and free  memory associated with running main().
 */

void cleanup() {
  SDL_Quit();
  if (g_obj != NULL) {
    free_object(g_obj);
    g_obj = NULL;
  }
} /* cleanup() */

/*
 *  main() is used to run the functions in hw12.c using command line arguments.
 *  It prints information related to running these functions.
 */

int main(int argc, char *argv[]) {
  SDL_Surface *screen = NULL;
  matrix trans = magnify(8, 8, 8);

  if (argc != 2) {
    printf("Specify a single object file on the command line.  e.g.: %s %s\n",
           argv[0], "cube.obj");
    return 1;
  }

  g_obj = read_object(argv[1]);

  if (g_obj == NULL) {
    printf("Error reading object.\n");
    return 1;
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

  atexit(SDL_Quit);

  screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
  if (screen == NULL) {
    fprintf(stderr, "Unable to set 640x480 video mode: %s\n", SDL_GetError());
    exit(1);
  }

  printf("Video depth is %d bytes.\n", screen->format->BytesPerPixel);

  g_obj->location = move_to(screen->w / 2, screen->h / 2, 0);

  sdl_lock(screen);
  draw_shade_background(screen);
  trans = magnify(8, 8, 8);
  render(screen, g_obj, trans);
  sdl_unlock(screen);
  SDL_Flip(screen);

  while(1) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      int press_x = -1;
      if (event.type == SDL_QUIT) {
        printf("Quit!\n");
        exit(0);
      }
      else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          printf("Escape!\n");
        }
        else if (event.key.keysym.sym == SDLK_a) {
          sdl_lock(screen);
          for (int r = 0; r > (-2 * M_PI); r -= (M_PI / 720.0)) {
            draw_line(screen, 320, 240, 320 + cos(r) * 240, 240 + sin(r) * 240,
                      255, 0, 0);
          }
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_r) {
          double r = 0.0;
          point2d array[10] = { 0 };
          sdl_lock(screen);
          draw_shade_background(screen);
          array[0].x = 320 + cos(r) * 240;
          array[0].y = 240 + sin(r) * 240;
          array[1].x = 320 + cos(r + M_PI_2) * 240;
          array[1].y = 240 + sin(r + M_PI_2) * 240;
          array[2].x = 320 + cos(r - M_PI_2) * 240;
          array[2].y = 240 + sin(r - M_PI_2) * 240;

          draw_polygon(screen, array, 3, 255, 0, 0);
          r += M_PI / 72.0;
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_o) {
          sdl_lock(screen);
          draw_shade_background(screen);
          trans = magnify(8, 8, 8);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_PAGEUP) {
          trans = mult(trans, magnify(2, 2, 2));
          sdl_lock(screen);
          draw_shade_background(screen);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_PAGEDOWN) {
          trans = mult(trans, magnify(.5, .5, .5));
          sdl_lock(screen);
          draw_shade_background(screen);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_UP) {
          trans = mult(trans, rotate_x(10));
          sdl_lock(screen);
          draw_shade_background(screen);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_DOWN) {
          trans = mult(trans, rotate_x(-10));
          sdl_lock(screen);
          draw_shade_background(screen);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_LEFT) {
          trans = mult(trans, rotate_y(-10));
          sdl_lock(screen);
          draw_shade_background(screen);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else if (event.key.keysym.sym == SDLK_RIGHT) {
          trans = mult(trans, rotate_y(10));
          sdl_lock(screen);
          draw_shade_background(screen);
          render(screen, g_obj, trans);
          sdl_unlock(screen);
          SDL_Flip(screen);
        }
        else {
          printf("Keypress!\n");
          cleanup();
          exit(0);
        }
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN) {
        press_x = event.button.x;
      }
      else if (event.type == SDL_MOUSEMOTION && press_x != -1) {
        SDL_Event event_array[100] = { 0 };

        trans = mult(trans, rotate_y(event.motion.xrel));
        trans = mult(trans, rotate_x(-event.motion.yrel));
        sdl_lock(screen);
        draw_shade_background(screen);
        render(screen, g_obj, trans);
        sdl_unlock(screen);
        SDL_Flip(screen);

        /* Absorb excessive mouse motion events. */

        while (SDL_PeepEvents(event_array, 2, SDL_PEEKEVENT, 0) > 1) {
          if (event_array[0].type == SDL_MOUSEMOTION &&
              event_array[1].type == SDL_MOUSEMOTION) {
            SDL_PeepEvents(event_array, 1, SDL_GETEVENT, 0);
          }
        }
      }
      else if (event.type == SDL_MOUSEBUTTONUP) {
        press_x = -1;
      }
    }
  }
} /* main() */
