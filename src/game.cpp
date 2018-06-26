#include "game.h"
#include "libretro.h"
#include <iostream>

int SCREEN_PITCH = 0;

//static cairo_surface_t *surface = NULL;
//static cairo_surface_t *static_surface = NULL;
//static cairo_t *ctx = NULL;


static uint16_t *frame_buf;
extern retro_environment_t environ_cb;

void game_init()
{
	/*
   frame_buf = calloc(SCREEN_HEIGHT, SCREEN_PITCH);
   srand(time(NULL));

   surface = cairo_image_surface_create_for_data(
            (unsigned char*)frame_buf, CAIRO_FORMAT_RGB16_565, SCREEN_WIDTH, SCREEN_HEIGHT,
            SCREEN_PITCH);

   ctx = cairo_create(surface);

   init_luts();
   init_static_surface();

   init_game();
   start_game();*/
}

void game_deinit()
{
	/*
   int i;

   for (i = 0; i < 13; i++)
   {
      cairo_pattern_destroy(color_lut[i]);
      color_lut[i] = NULL;
   }

   cairo_destroy(ctx);
   cairo_surface_destroy(surface);
   cairo_surface_destroy(static_surface);

   ctx     = NULL;
   surface = NULL;
   static_surface = NULL;

   if (frame_buf)
      free(frame_buf);
   frame_buf = NULL;*/

}
int game_init_pixelformat()
{
	
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      std::cout << "RGB565 is not supported.\n";
      return 0;
   }

   return 1;
}

void game_render() {

}