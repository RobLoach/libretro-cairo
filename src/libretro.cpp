#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include <cstdarg>
#include "libretro.h"

#include "game.h"

retro_log_printf_t log_cb;
retro_video_refresh_t video_cb;

retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

float frame_time = 0;



static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   va_list va;

   (void)level;

   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}


void retro_set_video_refresh(retro_video_refresh_t cb) {
	video_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb) {
	input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb) {
	input_state_cb = cb;
}


void retro_set_audio_sample(retro_audio_sample_t cb)
{
#if 0
   audio_cb = cb;
#endif
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
#if 0
   audio_batch_cb = cb;
#endif
}

/**
 * libretro callback; Sets up the environment based on the system variables.
 */
void retro_set_environment(retro_environment_t cb) {
   struct retro_log_callback logging;
   bool no_rom = true;

   environ_cb = cb;

   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;
}

/**
 * libretro callback; Updates the core option variables.
 */
static void update_variables(void) {
	std::cout << "update_variables()" << std::endl;
}

#ifdef __cplusplus
extern "C" {
#endif
/**
 * libretro callback; Load the labels for the input buttons.
 */
void init_descriptors() {
	struct retro_input_descriptor desc[] = {
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "Left Shoulder" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "Right Shoulder" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },

		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "Left Shoulder" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "Right Shoulder" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },

		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "Left Shoulder" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "Right Shoulder" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },

		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "Left Shoulder" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "Right Shoulder" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },

		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "Left Shoulder" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "Right Shoulder" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
		{ 4, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
		{ 0 },
	};

	environ_cb(RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, desc);
}
#ifdef __cplusplus
}
#endif

/**
 * libretro callback; Retrieve information about the core.
 */
void retro_get_system_info(struct retro_system_info *info) {
#ifndef GIT_VERSION
#define GIT_VERSION ""
#endif
	memset(info, 0, sizeof(*info));
	info->library_name = "Cairo Test";
	info->library_version  = "0.0.1" GIT_VERSION;
	info->need_fullpath = true;
	info->valid_extensions = "cairo";
	info->block_extract = true;
}

/**
 * libretro callback; Set the audio/video settings.
 */
void retro_get_system_av_info(struct retro_system_av_info *info) {
	std::cout << "retro_get_system_av_info" << std::endl;
	unsigned int width = 1920;
	unsigned int height = 1080;

	info->geometry.base_width   = width;
	info->geometry.base_height  = height;
	info->geometry.max_width    = width;
	info->geometry.max_height   = width;
	info->geometry.aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	info->timing.fps = 60.0;
	info->timing.sample_rate = 44100.0;
}

void retro_set_controller_port_device(unsigned port, unsigned device) {
	std::cout << "retro_set_controller_port_device" << std::endl;
	(void)port;
	(void)device;
}

/**
 * libretro callback; Return the amount of bytes required to save a state.
 */
size_t retro_serialize_size(void) {
	// Save states will be 10 kilobytes.
	return 10000;
}



/**
 * libretro callback; Serialize the current state to save a slot.
 */
bool retro_serialize(void *data, size_t size) {
	return false;
}

/**
 * libretro callback; Unserialize the given data and load the state.
 */
bool retro_unserialize(const void *data, size_t size) {
	return false;
}

/**
 * libretro callback; Reset the enabled cheats.
 */
void retro_cheat_reset(void) {
	// Nothing.
}

/**
 * libretro callback; Set the given cheat.
 */
void retro_cheat_set(unsigned index, bool enabled, const char *code) {
	(void)index;
	(void)enabled;
	(void)code;
}

/**
 * libretro callback; Step the core forwards a step.
 */
void frame_time_cb(retro_usec_t usec) {
	frame_time = usec / 1000000.0;
}

/**
 * libretro callback; Load the given game.
 */
bool retro_load_game(const struct retro_game_info *info) {
   struct retro_frame_time_callback frame_cb;
   init_descriptors();

   if (!game_init_pixelformat())
      return false;

   frame_cb.callback  = frame_time_cb;
   frame_cb.reference = 1000000 / 60;
   frame_cb.callback(frame_cb.reference);
   environ_cb(RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK, &frame_cb);

   (void)info;
   return true;
}

/**
 * libretro callback; Loads the given special game.
 */
bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) {
	std::cout << "retro_load_game_special" << std::endl;
	(void)game_type;
	(void)info;
	(void)num_info;
	return false;
}

/**
 * libretro callback; Unload the current game.
 */
void retro_unload_game(void) {
	std::cout << "retro_unload_game()" << std::endl;
}

/**
 * libretro callback; Retrieve the active region.
 */
unsigned retro_get_region(void) {
	return RETRO_REGION_NTSC;
}

/**
 * libretro callback; Get the libretro API version.
 */
unsigned retro_api_version(void) {
	return RETRO_API_VERSION;
}

/**
 * libretro callback; Get the given memory ID.
 */
void *retro_get_memory_data(unsigned id) {
	return NULL;
}

/**
 * libretro callback; Get the size of the given memory ID.
 */
size_t retro_get_memory_size(unsigned id) {
	return 0;
}

/**
 * libretro callback; Initialize the core.
 */
void retro_init(void) {

	std::cout << "retro_init()" << std::endl;
	game_init();
}

/**
 * libretro callback; Deinitialize the core.
 */
void retro_deinit(void) {
	std::cout << "retro_deinit()" << std::endl;
	game_deinit();
}

/**
 * libretro callback; The frontend requested to reset the game.
 */
void retro_reset(void) {
	std::cout << "retro_reset()" << std::endl;
}

/**
 * libretro callback; Run a game loop in the core.
 */
void retro_run(void) {
	game_render();
}