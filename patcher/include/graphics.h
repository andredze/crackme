#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//----------------------------------------------------------------------

#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//------------------------------------------------------------------//

const double EPSILON = 0.00000000000000001;

int CompareDoubles(double value1, double value2);

//----------------------------------------------------------------------

const char * const  TARGET_FILE      = "crackme.com";
const int           MAX_MESSAGE_SIZE = 64;
const int           PATCH_DELAY      = 400;

//------------------------------------------------------------------//

const int           WINDOW_W    = 550;
const int           WINDOW_H    = 400;
const char * const  WINDOW_NAME = "PATCHER";

//------------------------------------------------------------------//

const char * const  FONT_PATH           = "assets/DejaVuSans-Oblique.ttf";
const int           FONT_SIZE_BIG       = 22;
const int           FONT_SIZE_MEDIUM    = 18;
const int           FONT_SIZE_SMALL     = 16;

const char * const  TEXT_HEADER          = "КРЯК";
const char * const  START_STATUS_MESSAGE = "Нажми \"PATCH\" для взлома";

//------------------------------------------------------------------//

const SDL_Color COLOR_BG              = {18,  20,  30,  255};
const SDL_Color COLOR_HEADER          = {28,  100,  48,  255};
const SDL_Color COLOR_BORDER          = {55,  250,  90,  255};
const SDL_Color COLOR_TEXT            = {210, 215, 235, 255};
const SDL_Color COLOR_TARGET_MSG      = {210, 215, 235, 255};
const SDL_Color COLOR_BAR_BG          = {38,  40,  62,  255};
const SDL_Color COLOR_BAR_SHINE       = {140, 255, 180, 255};
const SDL_Color COLOR_BAR_FG          = {45,  210, 95,  255};
const SDL_Color COLOR_BUTTON_BORDER   = {140, 180, 125, 255};
const SDL_Color COLOR_BUTTON_PATCHING = {35,  65,  50, 255};
const SDL_Color COLOR_BUTTON_INACTIVE = {55,  110, 55,  255};
const SDL_Color COLOR_BUTTON_HOVERED  = {80,  145, 155, 255};
const SDL_Color COLOR_ERROR           = {220, 60,  60,  255};
const SDL_Color COLOR_OK              = {45,  210, 95,  255};

//------------------------------------------------------------------//

const int STATUS_X = 22;
const int STATUS_Y = 170;

//------------------------------------------------------------------//

typedef enum AppState
{
    STATE_INACTIVE,
    STATE_PATCHING,
    STATE_DONE,
    STATE_ERROR
}
AppState_t;

//------------------------------------------------------------------//

typedef struct Patcher
{
    SDL_Window*     window;
    SDL_Renderer*   renderer;
    FILE*           fp;

    TTF_Font*       font_big;
    TTF_Font*       font_medium;
    TTF_Font*       font_small;

    double          progress;
    AppState_t      state;
    const char*     status_msg;

    bool            window_is_active;
    SDL_Event       event;
    int             mouse_x;
    int             mouse_y;
    Uint32          last_ticks;
    SDL_Rect        button;
}
Patcher_t;

//------------------------------------------------------------------//

typedef enum PatchErr
{
    PATCH_SUCCESS,
    PATCH_FILE_ERROR,
    PATCH_NO_FONT
}
PatchErr_t;

//------------------------------------------------------------------//

PatchErr_t PatcherWaitTime(Patcher_t* patcher);
PatchErr_t PatcherUpdate  (Patcher_t* patcher);
PatchErr_t PatcherCtor    (Patcher_t* patcher);
void       PatcherDtor    (Patcher_t* patcher);
PatchErr_t RenderScreen   (Patcher_t* patcher);

//----------------------------------------------------------------------

#endif /* __GRAPHICS_H__ */
