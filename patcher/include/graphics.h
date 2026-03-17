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
    APP_AUDIO_LOAD_ERROR,
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

typedef struct GraphicsCtx
{
    SDL_Renderer*   rend;
    SDL_Window*     window;
    SDL_Surface*    screen_surface;
    TextureCtx_t    bg;
    TextureCtx_t    title;
    Mix_Music*      music;
}
GraphicsCtx_t;

//------------------------------------------------------------------//

typedef struct AppCtx
{
    bool        is_running;
    SDL_Event   event;
    FILE*       target_file;
}
AppCtx_t;

//------------------------------------------------------------------//

AppErr_t AppInit      (AppCtx_t* app, GraphicsCtx_t* gfx);
AppErr_t AppLoadMedia (AppCtx_t* app, GraphicsCtx_t* gfx);
AppErr_t AppDraw      (AppCtx_t* app, GraphicsCtx_t* gfx);
void     AppClose     (AppCtx_t* app, GraphicsCtx_t* gfx);

//------------------------------------------------------------------//

#endif /* __GRAPHICS_H__ */
