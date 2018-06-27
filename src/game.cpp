#include <iostream>

#include "game.h"
#include "libretro.h"

#include <cairo/cairo.h>

#include <cstring>

int SCREEN_PITCH = 0;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

static cairo_surface_t *surface = NULL;
static cairo_surface_t *static_surface = NULL;
static cairo_t *ctx = NULL;


static uint16_t *frame_buf;
extern retro_environment_t environ_cb;
extern retro_video_refresh_t video_cb;


void game_init()
{
	SCREEN_PITCH = cairo_format_stride_for_width(CAIRO_FORMAT_RGB16_565, SCREEN_WIDTH);

   frame_buf = (uint16_t*)calloc(SCREEN_HEIGHT, SCREEN_PITCH);

   surface = cairo_image_surface_create_for_data(
            (unsigned char*)frame_buf, CAIRO_FORMAT_RGB16_565, SCREEN_WIDTH, SCREEN_HEIGHT,
            SCREEN_PITCH);

   ctx = cairo_create(surface);

   //init_luts();
   //init_static_surface();

   //init_game();
   //start_game();
}

void game_deinit()
{
	/*
   int i;

   for (i = 0; i < 13; i++)
   {
      cairo_pattern_destroy(color_lut[i]);
      color_lut[i] = NULL;
   }*/

   cairo_destroy(ctx);
   cairo_surface_destroy(surface);
   cairo_surface_destroy(static_surface);

   ctx     = NULL;
   surface = NULL;
   static_surface = NULL;

   if (frame_buf)
      free(frame_buf);
   frame_buf = NULL;
}

int game_init_pixelformat() {
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      std::cout << "RGB565 is not supported.\n";
      return 0;
   }

   return 1;
}

int x = 0, y = 50;

void game_render() {

   // Clear the screen.
   cairo_set_source_rgb(ctx, 78.0 / 255.0, 205.0 / 255.0, 196.0 / 255.0);
   cairo_paint(ctx);

   // Draw a square.
   cairo_set_source_rgb(ctx, 255.0 / 255.0, 230.0 / 255.0, 109.0 / 255.0);
   cairo_rectangle(ctx, x++, y, 100, 100);
   cairo_fill(ctx);

   // Draw a square.
   cairo_set_source_rgba(ctx, 255.0 / 255.0, 107.0 / 255.0, 107.0 / 255.0, 0.7);
   cairo_arc(ctx, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, 50.0, 0.0, 2.0 * 3.14159);
   cairo_fill(ctx);

   // Test the freetype.
   cairo_select_font_face (ctx, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
   cairo_set_font_size (ctx, 90.0);
   cairo_move_to (ctx, 10.0, 135.0);
   cairo_show_text (ctx, "Hello");

   cairo_move_to (ctx, 70.0, 165.0);
   cairo_text_path (ctx, "World");
   cairo_set_source_rgb (ctx, 0.5, 0.5, 1);
   cairo_fill_preserve (ctx);
   cairo_set_source_rgb (ctx, 0, 0, 0);
   cairo_set_line_width (ctx, 2.56);
   cairo_stroke (ctx);

   // Set the frame buffer.
   video_cb(frame_buf, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_PITCH);
}