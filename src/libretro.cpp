#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include "libretro.h"
#include "audio/audio_mixer.h"

#include "game.h"

#define BUFSIZE 735 /* 44100/60 */
#define BUFSIZETIMESTWO 1470 /* 44100/60*2 */

retro_log_printf_t log_cb;
retro_video_refresh_t video_cb;

retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
retro_audio_sample_t audio_cb = NULL;
retro_audio_sample_batch_t audio_batch_cb = NULL;

float frame_time = 0;

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
	std::cout << "retro_set_audio_sample" << std::endl;
	audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
	std::cout << "retro_set_audio_sample_batch" << std::endl;
	audio_batch_cb = cb;
}

/**
 * libretro callback; Sets up the environment based on the system variables.
 */
void retro_set_environment(retro_environment_t cb) {
	std::cout << "retro_set_environment" << std::endl;
   struct retro_log_callback logging;
   bool no_rom = true;

   environ_cb = cb;

   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom);
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

		/*
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
		*/
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
	std::cout << "retro_get_system_info" << std::endl;
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
	unsigned int width = 640;
	unsigned int height = 480;

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
	return 0;
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
	std::cout << "retro_load_game" << std::endl;
	struct retro_frame_time_callback frame_cb;
	init_descriptors();

	if (!game_init_pixelformat())
		return false;

	frame_cb.callback  = frame_time_cb;
	frame_cb.reference = 1000000 / 60;
	frame_cb.callback(frame_cb.reference);
	environ_cb(RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK, &frame_cb);

	(void)info;

	game_load();
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
	game_unload();
}

/**
 * libretro callback; Retrieve the active region.
 */
unsigned retro_get_region(void) {
	std::cout << "retro_get_region" << std::endl;
	return RETRO_REGION_NTSC;
}

/**
 * libretro callback; Get the libretro API version.
 */
unsigned retro_api_version(void) {
	std::cout << "retro_api_version" << std::endl;
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
	audio_mixer_init(44100);
	game_init();
}

/**
 * libretro callback; Deinitialize the core.
 */
void retro_deinit(void) {
	std::cout << "retro_deinit()" << std::endl;
	game_deinit();
	audio_mixer_done();
}

/**
 * libretro callback; The frontend requested to reset the game.
 */
void retro_reset(void) {
	std::cout << "retro_reset()" << std::endl;
	game_reset();
}

bool first = false;
/**
 * libretro callback; Run a game loop in the core.
 */



void convert_float_to_s16(int16_t *out,
	const float *in, size_t samples)
{
	size_t i = 0;
	for (; i < samples; i++)
	{
		int32_t val = (int32_t)(in[i] * 0x8000);
		out[i] = (val > 0x7FFF) ? 0x7FFF :
			(val < -0x8000 ? -0x8000 : (int16_t)val);
	}
}


void retro_run(void) {
	if (!first) {
		first = true;
		std::cout << "retro_run" << std::endl;
	}

	// Update the audio.
	float samples[BUFSIZETIMESTWO] = { 0 };
	int16_t samples2[BUFSIZETIMESTWO] = { 0 };
	audio_mixer_mix(samples, BUFSIZE, 1.0, false);
	convert_float_to_s16(samples2,samples, BUFSIZETIMESTWO);
	audio_batch_cb(samples2, BUFSIZE);

	// Render the game.
	game_render();
}
