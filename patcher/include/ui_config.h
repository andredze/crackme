#ifndef __UI_CONFIG_H__
#define __UI_CONFIG_H__

//——————————————————————————————————————————————————————————————————————————————————————————

#include <SDL2/SDL.h>

//——————————————————————————————————————————————————————————————————————————————————————————

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 720;

const double SCREEN_RATIO = (double) SCREEN_WIDTH / SCREEN_HEIGHT;

const int PANORAMA_WIDTH  = 3072;
const int PANORAMA_HEIGHT = (int) (PANORAMA_WIDTH / SCREEN_RATIO);

const SDL_Rect FULL_SCREEN_RECT  = {.x = 0, .y = 0,   .w = SCREEN_WIDTH,   .h = SCREEN_HEIGHT};
const SDL_Rect PANORAMA_RECT     = {.x = 0, .y = 600, .w = PANORAMA_WIDTH, .h = PANORAMA_HEIGHT};

const char * const IMAGE_FILE_PATH_BACKGROUND = "assets/sand/panorama_0.png";

//------------------------------------------------------------------//

const int TITLE_IMAGE_WIDTH  = 2048;
const int TITLE_IMAGE_HEIGHT = 440;

const double TITLE_RATIO = (double) TITLE_IMAGE_WIDTH / TITLE_IMAGE_HEIGHT;

const int TITLE_SCREEN_WIDTH  = SCREEN_WIDTH / 2.5;
const int TITLE_SCREEN_HEIGHT = (int) (TITLE_SCREEN_WIDTH / TITLE_RATIO);

const int TITLE_SCREEN_POS_X = (SCREEN_WIDTH - TITLE_SCREEN_WIDTH) / 2;
const int TITLE_SCREEN_POS_Y = (TITLE_SCREEN_HEIGHT) / 2;

const SDL_Rect TITLE_SCREEN_RECT = {.x = TITLE_SCREEN_POS_X,
                                    .y = TITLE_SCREEN_POS_Y,
                                    .w = TITLE_SCREEN_WIDTH,
                                    .h = TITLE_SCREEN_HEIGHT};

const char * const IMAGE_FILE_PATH_TITLE = "assets/title.png";

//------------------------------------------------------------------//

const SDL_Color SDL_COLOR_RESET = {0x00, 0x00, 0x00, 0x00};

//------------------------------------------------------------------//

const char * const TARGET_FILE_NAME = "crackme.com";

const char * const MUSIC_FILE_PATH_CLASSIC = "assets/minecraft.mp3";

//------------------------------------------------------------------//

const char * const APP_WINDOW_NAME = "patcher";

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* __UI_CONFIG_H__ */
