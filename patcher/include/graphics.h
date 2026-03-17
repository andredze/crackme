#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//----------------------------------------------------------------------

#include <assert.h>
#include <stdio.h>
#include "my_printfs.h"
#include "ui_config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

//------------------------------------------------------------------//

const int DEFAULT_SOUND_FREQUENCY = 44100;
const int HARDWARE_CHANNELS_COUNT = 2;      // stereo
const int AUDIO_SAMPLE_SIZE       = 2048;   // audio chunk size

const double EPSILON = 0.00000000000000001;

int CompareDoubles(double value1, double value2);

//------------------------------------------------------------------//

typedef enum AppErr
{
    APP_SUCCESS,
    APP_SDL_INIT_ERROR,
    APP_SDL_WINDOW_ERROR,
    APP_SDL_RENDERER_ERROR,
    APP_IMG_INIT_ERROR,
    APP_AUDIO_INIT_ERROR,
    APP_FONT_INIT_ERROR,
    APP_AUDIO_LOAD_ERROR,
    APP_LOAD_FONT_ERROR,
    APP_SDL_SURFACE_ERROR,
    APP_SDL_TEXTURE_ERROR,
    APP_FILE_ERROR,
    APP_FONT_ERROR
}
AppErr_t;

//------------------------------------------------------------------//

typedef struct TextureCtx
{
    SDL_Rect        screen_rect;
    SDL_Texture*    image_texture;
    SDL_Rect        image_rect;
}
TextureCtx_t;

//------------------------------------------------------------------//

typedef enum ButtonState
{
    BUTTON_INACTIVE,
    BUTTON_HOVERED,
    BUTTON_PRESSED
}
ButtonState_t;

//------------------------------------------------------------------//

typedef struct Text
{
    const char*  text;
    SDL_Texture* texture;
    SDL_Rect     text_rect;
    SDL_Color    color;
    int          center_x;
    int          center_y;
}
Text_t;

//------------------------------------------------------------------//

typedef struct ButtonCtx
{
    TextureCtx_t    texture_ctx;
    ButtonState_t   state;
    Uint32          last_ticks;
    Text_t          text_ctx;
}
ButtonCtx_t;

//------------------------------------------------------------------//

typedef struct ProgressBar
{
    int progress;

    SDL_Rect     bar;
    SDL_Color    bar_color;

    SDL_Rect     fill;
    SDL_Color    fill_color;
}
ProgressBar_t;

//------------------------------------------------------------------//

typedef struct LoadScreenCtx
{
    TextureCtx_t    loading_bg;

    Text_t          title;
    Text_t          messages[LOAD_MESSAGES_COUNT];

    size_t          cur_message_number;

    Uint32          last_ticks;

    ProgressBar_t   progress_bar;
}
LoadScreenCtx_t;

//------------------------------------------------------------------//

typedef struct Theme
{
    TextureCtx_t    bg;
    Mix_Music*      music;
}
Theme_t;

//------------------------------------------------------------------//

typedef struct GraphicsCtx
{
    SDL_Renderer*   rend;
    SDL_Window*     window;
    SDL_Surface*    screen_surface;

    Theme_t         themes[THEMES_COUNT];
    int             curr_theme;
    Uint32          theme_last_ticks;

    TextureCtx_t    title;

    ButtonCtx_t     buttons[BUTTONS_COUNT];

    LoadScreenCtx_t load_screen;

    TTF_Font*       font;

    int             mouse_x;
    int             mouse_y;
}
GraphicsCtx_t;

//------------------------------------------------------------------//

typedef enum AppState
{
    APP_STATE_MAIN_MENU,
    APP_STATE_LOADING_SCREEN
}
AppState_t;

//------------------------------------------------------------------//

typedef struct AppCtx
{
    bool        is_running;
    SDL_Event   event;
    FILE*       target_file;
    AppState_t  state;
}
AppCtx_t;

//------------------------------------------------------------------//

AppErr_t AppInit       (AppCtx_t* app, GraphicsCtx_t* gfx);
AppErr_t AppLoadMedia  (AppCtx_t* app, GraphicsCtx_t* gfx);
AppErr_t AppUpdateState(AppCtx_t* app, GraphicsCtx_t* gfx);
AppErr_t AppDraw       (AppCtx_t* app, GraphicsCtx_t* gfx);
void     AppClose      (AppCtx_t* app, GraphicsCtx_t* gfx);

//------------------------------------------------------------------//

#endif /* __GRAPHICS_H__ */
