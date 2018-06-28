#ifndef GAME_H
#define GAME_H

void game_init();
void game_deinit();
void game_reset();
//void game_update(float delta, key_state_t *new_ks);
//void *game_data(void);
//void *game_save_data(void);
//unsigned game_data_size(void);
void game_render();
int game_init_pixelformat();
void game_unload();
#endif /* GAME_H */
