#include "graphics.h"
#include "crack.h"

//——————————————————————————————————————————————————————————————————————————————————————————

int CompareDoubles(double value1, double value2)
{
    assert(isfinite(value1));
    assert(isfinite(value2));

    return (abs(value1 - value2) < EPSILON);
}

//——————————————————————————————————————————————————————————————————————————————————————————

AppErr_t AppInit(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    DPRINTF("AppInit start\n");

    app->is_running = true;

    //------------------------------------------------------------------//

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        PRINTERR("Failed SDL_Init. SDL_Error: %s", SDL_GetError());
        return APP_SDL_INIT_ERROR;
    }

    //------------------------------------------------------------------//

    gfx->window = SDL_CreateWindow(APP_WINDOW_NAME,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
    if (gfx->window == NULL)
    {
        PRINTERR("Failed creating window. SDL_Error: %s", SDL_GetError());
        return APP_SDL_WINDOW_ERROR;
    }

    //------------------------------------------------------------------//

    gfx->rend = SDL_CreateRenderer(gfx->window, -1, SDL_RENDERER_ACCELERATED);

    if (gfx->rend == NULL)
    {
        PRINTERR("Failed to create renderer. SDL_Error: %s", SDL_GetError());
        return APP_SDL_RENDERER_ERROR;
    }

    //------------------------------------------------------------------//

    SDL_SetRenderDrawColor(gfx->rend,
                           SDL_COLOR_RESET.r,
                           SDL_COLOR_RESET.g,
                           SDL_COLOR_RESET.b,
                           SDL_COLOR_RESET.a);

    gfx->screen_surface = SDL_GetWindowSurface(gfx->window);

    //------------------------------------------------------------------//

    int image_flags = IMG_INIT_PNG;

    if ((IMG_Init(image_flags) & image_flags) == 0)
    {
        PRINTERR("Failed IMG_Init. IMG_Error: %s", IMG_GetError());
        return APP_IMG_INIT_ERROR;
    }

    //------------------------------------------------------------------//

    if (Mix_OpenAudio(DEFAULT_SOUND_FREQUENCY, MIX_DEFAULT_FORMAT,
                      HARDWARE_CHANNELS_COUNT, AUDIO_SAMPLE_SIZE) < 0)
    {
        PRINTERR("Failed Mixer init. MIX_Error: %s", Mix_GetError());
        return APP_AUDIO_INIT_ERROR;
    }

    return APP_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————————————————

static SDL_Surface* AppLoadSurfaceFromImage(GraphicsCtx_t* gfx, const char* image_file_path)
{
    assert(gfx);
    assert(image_file_path);

    SDL_Surface* optimized_surface = NULL;

    SDL_Surface* loaded_surface = IMG_Load(image_file_path);

    if (loaded_surface == NULL)
    {
        PRINTERR("Failed to load surface from image %s. IMG_Error: %s",
                 image_file_path, IMG_GetError());
        return NULL;
    }

    optimized_surface = SDL_ConvertSurface(loaded_surface, gfx->screen_surface->format, 0);

    SDL_FreeSurface(loaded_surface);

    if (optimized_surface == NULL)
    {
        PRINTERR("Failed to optimize image %s. IMG_Error: %s",
                 image_file_path, IMG_GetError());
        return NULL;
    }

    return optimized_surface;
}

//------------------------------------------------------------------//

static SDL_Texture* AppLoadTextureFromImage(GraphicsCtx_t* gfx, const char* image_file_path)
{
    assert(gfx);
    assert(image_file_path);

    SDL_Texture* new_texture = NULL;

    SDL_Surface* loaded_surface = IMG_Load(image_file_path);

    if (loaded_surface == NULL)
    {
        PRINTERR("Failed to load surface from image %s. IMG_Error: %s",
                 image_file_path, IMG_GetError());
        return NULL;
    }

    new_texture = SDL_CreateTextureFromSurface(gfx->rend, loaded_surface);

    SDL_FreeSurface(loaded_surface);

    if (new_texture == NULL)
    {
        PRINTERR("Failed to create a texture from image %s. IMG_Error: %s",
                 image_file_path, IMG_GetError());
        return NULL;
    }

    return new_texture;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadTexture(GraphicsCtx_t*       gfx,
                               TextureCtx_t*        texture_ctx,
                               const char * const   image_file_path,
                               const SDL_Rect*      image_rect,
                               const SDL_Rect*      screen_rect)
{
    assert(gfx);
    assert(texture_ctx);

    texture_ctx->image_texture = AppLoadTextureFromImage(gfx, image_file_path);

    if (texture_ctx->image_texture == NULL)
    {
        return APP_SDL_TEXTURE_ERROR;
    }
    if (screen_rect != NULL)
    {
        texture_ctx->screen_rect = *screen_rect;
    }
    if (image_rect != NULL)
    {
        texture_ctx->image_rect = *image_rect;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

AppErr_t AppLoadMedia(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    gfx->music = Mix_LoadMUS(MUSIC_FILE_PATH_CLASSIC);

    if (gfx->music == NULL)
    {
        PRINTERR("Failed to load music %s. Mix_Error: %s", MUSIC_FILE_PATH_CLASSIC, Mix_GetError());
        return APP_AUDIO_LOAD_ERROR;
    }

    if (AppLoadTexture(gfx, &gfx->bg, IMAGE_FILE_PATH_BACKGROUND, &PANORAMA_RECT, &FULL_SCREEN_RECT))
    {
        return APP_SDL_TEXTURE_ERROR;
    }
    if (AppLoadTexture(gfx, &gfx->title, IMAGE_FILE_PATH_TITLE, NULL, &TITLE_SCREEN_RECT))
    {
        return APP_SDL_TEXTURE_ERROR;
    }

    return APP_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————————————————

AppErr_t AppDraw(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    if (Mix_PlayingMusic() == 0)
    {
        Mix_PlayMusic(gfx->music, -1); // -1 == loop music
    }

    SDL_RenderClear(gfx->rend);

    SDL_RenderCopy(gfx->rend,
                   gfx->bg.image_texture,
                   &gfx->bg.image_rect,
                   &gfx->bg.screen_rect);

    SDL_RenderCopy(gfx->rend,
                   gfx->title.image_texture,
                   NULL,
                   &gfx->title.screen_rect);

    SDL_RenderPresent(gfx->rend);

    return APP_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————————————————

void AppClose(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    Mix_FreeMusic(gfx->music);
    gfx->music = NULL;

    SDL_DestroyTexture(gfx->bg.image_texture);
    gfx->bg.image_texture = NULL;

    SDL_FreeSurface(gfx->screen_surface);
    gfx->screen_surface = NULL;

    SDL_DestroyRenderer(gfx->rend);
    gfx->rend = NULL;

    SDL_DestroyWindow(gfx->window);
    gfx->window = NULL;

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

//——————————————————————————————————————————————————————————————————————————————————————————
