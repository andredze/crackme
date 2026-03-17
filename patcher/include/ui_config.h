#ifndef __UI_CONFIG_H__
#define __UI_CONFIG_H__

//——————————————————————————————————————————————————————————————————————————————————————————

#include <SDL2/SDL.h>

//——————————————————————————————————————————————————————————————————————————————————————————

const Uint32 THEME_CHANGE_DELAY        = 500;
const Uint32 PRESS_BUTTON_TIME_DELAY   = 100;
const Uint32 LOADING_SCREEN_FRAME_TIME = 750;

//------------------------------------------------------------------//

const char * const FONT_FILE_PATH = "assets/font.ttf";
const int          FONT_SIZE      = 28;

//------------------------------------------------------------------//

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 720;

const double SCREEN_RATIO = (double) SCREEN_WIDTH / SCREEN_HEIGHT;

const int PANORAMA_WIDTH  = 3072;
const int PANORAMA_HEIGHT = (int) (PANORAMA_WIDTH / SCREEN_RATIO);

const SDL_Rect FULL_SCREEN_RECT  = {.x = 0, .y = 0,   .w = SCREEN_WIDTH,   .h = SCREEN_HEIGHT};
const SDL_Rect PANORAMA_RECT     = {.x = 0, .y = 600, .w = PANORAMA_WIDTH, .h = PANORAMA_HEIGHT};

//------------------------------------------------------------------//

const int PROGRESS_BARS_COUNT = 10;

const int PROGRESS_BAR_WIDTH  = (int) (SCREEN_WIDTH / 2.5);
const int PROGRESS_BAR_HEIGHT = 15;
const int PROGRESS_BAR_POS_X  = (SCREEN_WIDTH  - PROGRESS_BAR_WIDTH) / 2;
const int PROGRESS_BAR_POS_Y  = (SCREEN_HEIGHT + 50) / 2;

const SDL_Rect PROGRESS_BAR_SCREEN_RECT = {.x = PROGRESS_BAR_POS_X,
                                           .y = PROGRESS_BAR_POS_Y,
                                           .w = PROGRESS_BAR_WIDTH,
                                           .h = PROGRESS_BAR_HEIGHT};

const SDL_Color PROGRESS_BAR_COLOR     = {0x80, 0x80, 0x80, 0xFF};
const SDL_Color PROGRESS_BAR_FILLCOLOR = {0x80, 0xFF, 0x80, 0xFF};

//------------------------------------------------------------------//

typedef struct ThemeCase
{
    const char* const bg_file_path;
    const char* const music_file_path;
}
ThemeCase_t;

//------------------------------------------------------------------//

const ThemeCase_t THEMES_TABLE[] =
{
    {"assets/sand/panorama_0.png",    "assets/minecraft.mp3"},
    {"assets/classic/panorama_0.png", "assets/haggstrom.mp3"},
    {"assets/sunset/panorama_0.png",  "assets/moogcity.mp3"}
};

const size_t THEMES_COUNT = sizeof(THEMES_TABLE) / sizeof(THEMES_TABLE[0]);

//------------------------------------------------------------------//

const SDL_Color SDL_COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};

//------------------------------------------------------------------//

const int LOADING_BG_WIDTH  = 854;
const int LOADING_BG_HEIGHT = 480;

const SDL_Rect LOADING_BG_RECT = {.x = 0, .y = 0, .w = LOADING_BG_WIDTH, .h = LOADING_BG_HEIGHT};

const char * const IMAGE_FILE_PATH_LOADING_BACKGROUND = "assets/loading_bg.png";

//------------------------------------------------------------------//

const char * const LOAD_MESSAGE_TEXT_TABLE[] =
{
    "Generating terrain...",
    "Opening crackme.com file...",
    "Searching for password verifier...",
    "Replacing JZ with NOPS...",
    "Deleting French...",
    "Deleting system32...",
    "Mining Bitcoin...",
    "Wasting time...",
    "Overflowing buffers...",
    "Stealing passwords...",
    "Done!"
};

const size_t LOAD_MESSAGES_COUNT = sizeof(LOAD_MESSAGE_TEXT_TABLE) / sizeof(LOAD_MESSAGE_TEXT_TABLE[0]);

//------------------------------------------------------------------//

const SDL_Color LOAD_MESSAGE_COLOR  = SDL_COLOR_WHITE;
const int LOAD_MESSAGE_CENTER_POS_Y = SCREEN_HEIGHT * 5 / 10;
const int LOAD_MESSAGE_CENTER_POS_X = SCREEN_WIDTH / 2;

const char * const LOAD_TITLE_TEXT = "Patching CRACKME.COM level";
const SDL_Color LOAD_TITLE_COLOR   = SDL_COLOR_WHITE;
const int LOAD_TITLE_CENTER_POS_Y  = SCREEN_HEIGHT * 4 / 10;
const int LOAD_TITLE_CENTER_POS_X  = SCREEN_WIDTH / 2;

//------------------------------------------------------------------//

const int TITLE_IMAGE_WIDTH  = 2048;
const int TITLE_IMAGE_HEIGHT = 440;

const double TITLE_RATIO = (double) TITLE_IMAGE_WIDTH / TITLE_IMAGE_HEIGHT;

const int TITLE_SCREEN_WIDTH  = (int) (SCREEN_WIDTH / 2.1);
const int TITLE_SCREEN_HEIGHT = (int) (TITLE_SCREEN_WIDTH / TITLE_RATIO);

const int TITLE_SCREEN_POS_X = (SCREEN_WIDTH - TITLE_SCREEN_WIDTH) / 2;
const int TITLE_SCREEN_POS_Y = (5 * TITLE_SCREEN_HEIGHT / 8);

const SDL_Rect TITLE_SCREEN_RECT = {.x = TITLE_SCREEN_POS_X,
                                    .y = TITLE_SCREEN_POS_Y,
                                    .w = TITLE_SCREEN_WIDTH,
                                    .h = TITLE_SCREEN_HEIGHT};

const char * const IMAGE_FILE_PATH_TITLE = "assets/title.png";

//------------------------------------------------------------------//

const int BUTTON_IMAGE_WIDTH  = 200;
const int BUTTON_IMAGE_HEIGHT = 20;

const int BUTTON_IMAGE_POS_X = 0;

const int BUTTON_IMAGE_POS_Y_PRESSED  = 46;
const int BUTTON_IMAGE_POS_Y_INACTIVE = 66;
const int BUTTON_IMAGE_POS_Y_HOVERED  = 86;

const SDL_Rect BUTTON_IMAGE_RECT_INACTIVE = {.x = BUTTON_IMAGE_POS_X,
                                             .y = BUTTON_IMAGE_POS_Y_INACTIVE,
                                             .w = BUTTON_IMAGE_WIDTH,
                                             .h = BUTTON_IMAGE_HEIGHT};

const char * const IMAGE_FILE_PATH_BUTTON = "assets/button.png";

//------------------------------------------------------------------//

const double BUTTON_RATIO = (double) BUTTON_IMAGE_WIDTH / BUTTON_IMAGE_HEIGHT;

const int BUTTON_SCREEN_WIDTH  = (int) (SCREEN_WIDTH / 2);
const int BUTTON_SCREEN_HEIGHT = (int) (BUTTON_SCREEN_WIDTH / BUTTON_RATIO);

const int BUTTON_SCREEN_POS_X  = (SCREEN_WIDTH - BUTTON_SCREEN_WIDTH)/ 2;

const int BUTTON_SCREEN_POS_Y_CRACK = (SCREEN_HEIGHT * 5) / 10;
const int BUTTON_DISTANCE           = (int) (BUTTON_SCREEN_HEIGHT * 2.5 / 2);
const int BUTTON_SCREEN_POS_Y_THEME = BUTTON_SCREEN_POS_Y_CRACK + BUTTON_DISTANCE;
const int BUTTON_SCREEN_POS_Y_CLOSE = BUTTON_SCREEN_POS_Y_THEME + BUTTON_DISTANCE;

//------------------------------------------------------------------//

const SDL_Rect CRACK_BUTTON_SCREEN_RECT = {.x = BUTTON_SCREEN_POS_X,
                                           .y = BUTTON_SCREEN_POS_Y_CRACK,
                                           .w = BUTTON_SCREEN_WIDTH,
                                           .h = BUTTON_SCREEN_HEIGHT};

const SDL_Rect THEME_BUTTON_SCREEN_RECT = {.x = BUTTON_SCREEN_POS_X,
                                           .y = BUTTON_SCREEN_POS_Y_THEME,
                                           .w = BUTTON_SCREEN_WIDTH,
                                           .h = BUTTON_SCREEN_HEIGHT};

const SDL_Rect CLOSE_BUTTON_SCREEN_RECT = {.x = BUTTON_SCREEN_POS_X,
                                           .y = BUTTON_SCREEN_POS_Y_CLOSE,
                                           .w = BUTTON_SCREEN_WIDTH,
                                           .h = BUTTON_SCREEN_HEIGHT};

//------------------------------------------------------------------//

typedef struct ButtonCase
{
    const char* text;
    SDL_Color   text_color;
    SDL_Rect    screen_rect;
}
ButtonCase_t;

//------------------------------------------------------------------//

const int LETTER_SIZE = 15;

const ButtonCase_t BUTTON_CASES_TABLE[] =
{
    {.text = "Crack"        , .text_color = SDL_COLOR_WHITE, .screen_rect = CRACK_BUTTON_SCREEN_RECT},
    {.text = "Switch themes", .text_color = SDL_COLOR_WHITE, .screen_rect = THEME_BUTTON_SCREEN_RECT},
    {.text = "Close"        , .text_color = SDL_COLOR_WHITE, .screen_rect = CLOSE_BUTTON_SCREEN_RECT}
};

const int BUTTONS_COUNT = (int) sizeof(BUTTON_CASES_TABLE) / sizeof(BUTTON_CASES_TABLE[0]);

//------------------------------------------------------------------//

const SDL_Color SDL_COLOR_RESET = {0x00, 0x00, 0x00, 0x00};

//------------------------------------------------------------------//

const char * const TARGET_FILE_NAME = "crackme.com";

//------------------------------------------------------------------//

const char * const APP_WINDOW_NAME = "patcher";

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* __UI_CONFIG_H__ */
