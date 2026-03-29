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

    app->state = APP_STATE_MAIN_MENU;
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

    //------------------------------------------------------------------//

    if (TTF_Init() == -1)
    {
        PRINTERR("Failed to init fonts. TTF_Error: %s", TTF_GetError());
        return APP_FONT_INIT_ERROR;
    }

    //------------------------------------------------------------------//

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

static AppErr_t AppLoadText(GraphicsCtx_t* gfx, Text_t* text_ctx)
{
    assert(gfx);
    assert(text_ctx);

    SDL_Surface* text_surface = TTF_RenderText_Solid(gfx->font, text_ctx->text,
                                                     text_ctx->color);

    if (text_surface == NULL)
    {
        PRINTERR("Failed to make surface from text %s. TTF_Error: %s", text_ctx->text, TTF_GetError());
        return APP_SDL_SURFACE_ERROR;
    }

    text_ctx->texture = SDL_CreateTextureFromSurface(gfx->rend, text_surface);

    if (text_ctx->texture == NULL)
    {
        PRINTERR("Failed to make texture for text %s. SDL_Error: %s", text_ctx->text, SDL_GetError());
        return APP_SDL_TEXTURE_ERROR;
    }

    text_ctx->text_rect.w = LETTER_SIZE * (int) strlen(text_ctx->text);
    text_ctx->text_rect.h = (int) (((double) text_ctx->text_rect.w / text_surface->w) * text_surface->h);

    text_ctx->text_rect.x = text_ctx->center_x - text_ctx->text_rect.w / 2;
    text_ctx->text_rect.y = text_ctx->center_y - text_ctx->text_rect.h / 2;

    DPRINTF("text: %s, w = %d, h = %d, x = %d, y = %d\n",
            text_ctx->text,
            text_ctx->text_rect.w,
            text_ctx->text_rect.h,
            text_ctx->text_rect.x,
            text_ctx->text_rect.y);

    SDL_FreeSurface(text_surface);

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadButton(GraphicsCtx_t* gfx, ButtonCtx_t* button, int button_number)
{
    assert(gfx);
    assert(button);

    if (AppLoadTexture(gfx, &button->texture_ctx,
                       IMAGE_FILE_PATH_BUTTON,
                       &BUTTON_IMAGE_RECT_INACTIVE,
                       &BUTTON_CASES_TABLE[button_number].screen_rect))
    {
        return APP_SDL_TEXTURE_ERROR;
    }

    button->text_ctx.center_x = button->texture_ctx.screen_rect.x + button->texture_ctx.screen_rect.w / 2;
    button->text_ctx.center_y = button->texture_ctx.screen_rect.y + button->texture_ctx.screen_rect.h / 2;
    button->text_ctx.text     = BUTTON_CASES_TABLE[button_number].text;
    button->text_ctx.color    = BUTTON_CASES_TABLE[button_number].text_color;

    AppErr_t error = APP_SUCCESS;

    if ((error = AppLoadText(gfx, &button->text_ctx)))
    {
        return error;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadMenu(GraphicsCtx_t* gfx)
{
    assert(gfx);

    AppErr_t error = APP_SUCCESS;

    for (int i = 0; i < BUTTONS_COUNT; i++)
    {
        if ((error = AppLoadButton(gfx, &gfx->buttons[i], i)))
        {
            return error;
        }
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadMain(GraphicsCtx_t* gfx)
{
    assert(gfx);

    for (size_t i = 0; i < THEMES_COUNT; i++)
    {
        if (AppLoadTexture(gfx, &gfx->themes[i].bg, THEMES_TABLE[i].bg_file_path,
                           &PANORAMA_RECT, &FULL_SCREEN_RECT))
        {
            return APP_SDL_TEXTURE_ERROR;
        }
    }

    if (AppLoadTexture(gfx, &gfx->title, IMAGE_FILE_PATH_TITLE,
                       NULL, &TITLE_SCREEN_RECT))
    {
        return APP_SDL_TEXTURE_ERROR;
    }

    AppErr_t error = APP_SUCCESS;

    if ((error = AppLoadMenu(gfx)))
    {
        return error;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadMessage(GraphicsCtx_t* gfx, Text_t* message)
{
    assert(gfx);
    assert(message);

    AppErr_t error = APP_SUCCESS;

    message->center_x = LOAD_MESSAGE_CENTER_POS_X;
    message->center_y = LOAD_MESSAGE_CENTER_POS_Y;
    message->color    = LOAD_MESSAGE_COLOR;

    if ((error = AppLoadText(gfx, message)))
    {
        return error;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadProgressBar(GraphicsCtx_t* gfx)
{
    assert(gfx);

    gfx->load_screen.progress_bar.bar    = PROGRESS_BAR_SCREEN_RECT;
    gfx->load_screen.progress_bar.fill   = PROGRESS_BAR_SCREEN_RECT;
    gfx->load_screen.progress_bar.fill.w = 0;

    gfx->load_screen.progress_bar.bar_color  = PROGRESS_BAR_COLOR;
    gfx->load_screen.progress_bar.fill_color = PROGRESS_BAR_FILLCOLOR;

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppLoadLoadingScreen(GraphicsCtx_t* gfx)
{
    assert(gfx);

    AppErr_t error = APP_SUCCESS;

    if (AppLoadTexture(gfx, &gfx->load_screen.loading_bg,
                       IMAGE_FILE_PATH_LOADING_BACKGROUND,
                       &LOADING_BG_RECT, &FULL_SCREEN_RECT))
    {
        return APP_SDL_TEXTURE_ERROR;
    }

    for (size_t i = 0; i < LOAD_MESSAGES_COUNT; i++)
    {
        gfx->load_screen.messages[i].text = LOAD_MESSAGE_TEXT_TABLE[i];

        if ((error = AppLoadMessage(gfx, &gfx->load_screen.messages[i])))
        {
            return error;
        }
    }

    gfx->load_screen.title.text     = LOAD_TITLE_TEXT;
    gfx->load_screen.title.center_x = LOAD_TITLE_CENTER_POS_X;
    gfx->load_screen.title.center_y = LOAD_TITLE_CENTER_POS_Y;
    gfx->load_screen.title.color    = LOAD_TITLE_COLOR;

    if ((error = AppLoadText(gfx, &gfx->load_screen.title)))
    {
        return error;
    }
    if ((error = AppLoadProgressBar(gfx)))
    {
        return error;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

AppErr_t AppLoadMedia(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    for (size_t i = 0; i < THEMES_COUNT; i++)
    {
        gfx->themes[i].music = Mix_LoadMUS(THEMES_TABLE[i].music_file_path);

        if (gfx->themes[i].music == NULL)
        {
            PRINTERR("Failed to load music %s. Mix_Error: %s",
                     THEMES_TABLE[i].music_file_path, Mix_GetError());
            return APP_AUDIO_LOAD_ERROR;
        }
    }

    Mix_PlayMusic(gfx->themes[gfx->curr_theme].music, -1);

    gfx->font = TTF_OpenFont(FONT_FILE_PATH, FONT_SIZE);

    if (gfx->font == NULL)
    {
        PRINTERR("Failed to open font %s. Error: %s", FONT_FILE_PATH, TTF_GetError());
        return APP_LOAD_FONT_ERROR;
    }

    AppErr_t error = APP_SUCCESS;

    if ((error = AppLoadMain(gfx)))
    {
        return error;
    }
    if ((error = AppLoadLoadingScreen(gfx)))
    {
        return error;
    }

    return APP_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————————————————

static bool AppMouseIsInsideButton(GraphicsCtx_t* gfx, ButtonCtx_t* button)
{
    assert(gfx);
    assert(button);

    if (!(gfx->mouse_x >= button->texture_ctx.screen_rect.x))
        return false;

    if (!(gfx->mouse_x <= button->texture_ctx.screen_rect.x +
                          button->texture_ctx.screen_rect.w))
        return false;

    if (!(gfx->mouse_y >= button->texture_ctx.screen_rect.y))
        return false;

    if (!(gfx->mouse_y <= button->texture_ctx.screen_rect.y +
                          button->texture_ctx.screen_rect.h))
        return false;

    return true;
}

//------------------------------------------------------------------//

static bool AppIsOnCooldown(Uint32* last_activation_ms, Uint32 cooldown_ms)
{
    Uint32 current_ms = SDL_GetTicks();

    if (current_ms - *last_activation_ms >= cooldown_ms)
    {
        *last_activation_ms = current_ms;
        return false;
    }

    return true;
}

//------------------------------------------------------------------//

static AppErr_t AppUpdateButtonState(AppCtx_t* app, GraphicsCtx_t* gfx, ButtonCtx_t* button)
{
    assert(app);
    assert(gfx);

    bool inside = AppMouseIsInsideButton(gfx, button);

    if (button->state == BUTTON_PRESSED)
    {
        bool is_on_cooldown = AppIsOnCooldown(&button->last_ticks, PRESS_BUTTON_TIME_DELAY);

        if (is_on_cooldown)
            return APP_SUCCESS;
    }

    if (!inside)
    {
        button->state = BUTTON_INACTIVE;

        return APP_SUCCESS;
    }

    if (app->event.type == SDL_MOUSEBUTTONDOWN)
    {
        button->state      = BUTTON_PRESSED;
        button->last_ticks = SDL_GetTicks();

        return APP_SUCCESS;
    }

    button->state = BUTTON_HOVERED;

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppUpdateLoadingScreen(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    bool is_on_cooldown = AppIsOnCooldown(&gfx->load_screen.last_ticks, LOADING_SCREEN_FRAME_TIME);
    
    if (is_on_cooldown)
    {
        return APP_SUCCESS;
    }

    if (gfx->load_screen.progress_bar.progress >= 100)
    {
        app->state = APP_STATE_MAIN_MENU;
    
        gfx->load_screen.progress_bar.progress = 0;
        gfx->load_screen.cur_message_number    = 0;
    
        return APP_SUCCESS;
    }

    gfx->load_screen.progress_bar.progress += 10;
    gfx->load_screen.cur_message_number    += 1;

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppUpdateMainMenu(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    AppErr_t error = APP_SUCCESS;

    if (!(app->event.type == SDL_MOUSEMOTION     ||
          app->event.type == SDL_MOUSEBUTTONDOWN ||
          app->event.type == SDL_MOUSEBUTTONUP))
    {
        return APP_SUCCESS;
    }

    SDL_GetMouseState(&gfx->mouse_x, &gfx->mouse_y);

    for (int i = 0; i < BUTTONS_COUNT; i++)
    {
        if ((error = AppUpdateButtonState(app, gfx, &gfx->buttons[i])))
                return error;
    }

    if (gfx->buttons[0].state == BUTTON_PRESSED)
    {
        app->state = APP_STATE_LOADING_SCREEN;

        if ((error = MakePatch(app, gfx)))
        {
            return error;
        }
    }
    if (gfx->buttons[1].state == BUTTON_PRESSED)
    {
        bool make_delay = AppIsOnCooldown(&gfx->theme_last_ticks, THEME_CHANGE_DELAY);

        if (make_delay)
            return APP_SUCCESS;

        gfx->curr_theme += 1;
        gfx->curr_theme %= (int) THEMES_COUNT;

        Mix_PlayMusic(gfx->themes[gfx->curr_theme].music, -1); // -1 == loop music
    }
    if (gfx->buttons[2].state == BUTTON_PRESSED)
    {
        app->is_running = false;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

static AppErr_t AppUpdateScreen(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    AppErr_t error = APP_SUCCESS;

    if (app->state == APP_STATE_LOADING_SCREEN)
    {
        if ((error = AppUpdateLoadingScreen(app, gfx)))
            return error;
    }
    else
    {
        if ((error = AppUpdateMainMenu(app, gfx)))
            return error;
    }

    return APP_SUCCESS;
}

//------------------------------------------------------------------//

AppErr_t AppUpdateState(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    AppErr_t error = APP_SUCCESS;

    bool entered_while = false;

    while (SDL_PollEvent(&app->event) != 0)
    {
        entered_while = true;

        if (app->event.type == SDL_QUIT)
        {
            app->is_running = false;
        }

        if ((error = AppUpdateScreen(app, gfx)))
            return error;
    }

    if (entered_while)
        return APP_SUCCESS;

    if ((error = AppUpdateScreen(app, gfx)))
        return error;

    return APP_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————————————————

static void RenderButton(AppCtx_t* app, SDL_Renderer* rend, ButtonCtx_t* button)
{
    assert(app);
    assert(button);

    switch (button->state)
    {
        case BUTTON_INACTIVE:
            button->texture_ctx.image_rect.y = BUTTON_IMAGE_POS_Y_INACTIVE;
            break;

        case BUTTON_HOVERED:
            button->texture_ctx.image_rect.y = BUTTON_IMAGE_POS_Y_HOVERED;
            break;

        case BUTTON_PRESSED:
            button->texture_ctx.image_rect.y = BUTTON_IMAGE_POS_Y_PRESSED;
            break;

        default:
            PRINTERR("Unknown button state!");
            return;
    }

    SDL_RenderCopy(rend,
                   button->texture_ctx.image_texture,
                   &button->texture_ctx.image_rect,
                   &button->texture_ctx.screen_rect);

    SDL_RenderCopy(rend,
                   button->text_ctx.texture,
                   NULL,
                   &button->text_ctx.text_rect);

    return;
}

//------------------------------------------------------------------//

static void RenderMenu(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    for (int i = 0; i < BUTTONS_COUNT; i++)
    {
        RenderButton(app, gfx->rend, &gfx->buttons[i]);
    }
}

//------------------------------------------------------------------//

static void RenderMain(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    SDL_RenderCopy(gfx->rend,
                   gfx->themes[gfx->curr_theme].bg.image_texture,
                   &gfx->themes[gfx->curr_theme].bg.image_rect,
                   &gfx->themes[gfx->curr_theme].bg.screen_rect);

    SDL_RenderCopy(gfx->rend,
                   gfx->title.image_texture,
                   NULL,
                   &gfx->title.screen_rect);

    RenderMenu(app, gfx);
}

//------------------------------------------------------------------//

static void RenderProgressBar(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    SDL_Color bar_color = gfx->load_screen.progress_bar.bar_color;

    SDL_SetRenderDrawColor(gfx->rend, bar_color.r, bar_color.g, bar_color.b, bar_color.a);
    SDL_RenderFillRect(gfx->rend, &gfx->load_screen.progress_bar.bar);

    gfx->load_screen.progress_bar.fill.w = gfx->load_screen.progress_bar.bar.w *
                                           gfx->load_screen.progress_bar.progress / 100;

    SDL_Color fill_color = gfx->load_screen.progress_bar.fill_color;

    SDL_SetRenderDrawColor(gfx->rend, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
    SDL_RenderFillRect(gfx->rend, &gfx->load_screen.progress_bar.fill);
}

//------------------------------------------------------------------//

static void RenderLoadingScreen(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    SDL_RenderCopy(gfx->rend,
                   gfx->load_screen.loading_bg.image_texture,
                   &gfx->load_screen.loading_bg.image_rect,
                   &gfx->load_screen.loading_bg.screen_rect);

    SDL_RenderCopy(gfx->rend,
                   gfx->load_screen.title.texture,
                   NULL,
                   &gfx->load_screen.title.text_rect);

    if (gfx->load_screen.cur_message_number >= LOAD_MESSAGES_COUNT)
    {
        PRINTERR("Error: cur_message_number = %zu (too big)\n", 
                 gfx->load_screen.cur_message_number);
        return;
    }

    SDL_RenderCopy(gfx->rend,
                   gfx->load_screen.messages[gfx->load_screen.cur_message_number].texture,
                   NULL,
                   &gfx->load_screen.messages[gfx->load_screen.cur_message_number].text_rect);

    RenderProgressBar(app, gfx);
}

//------------------------------------------------------------------//

AppErr_t AppDraw(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    SDL_RenderClear(gfx->rend);

    if (app->state == APP_STATE_MAIN_MENU)
    {
        RenderMain(app, gfx);
    }
    else
    {
        RenderLoadingScreen(app, gfx);
    }

    SDL_RenderPresent(gfx->rend);

    return APP_SUCCESS;
}

//——————————————————————————————————————————————————————————————————————————————————————————

void AppClose(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    Mix_HaltMusic();

    TTF_CloseFont(gfx->font);
    gfx->font = NULL;

    for (int i = 0; i < BUTTONS_COUNT; i++)
    {
        SDL_DestroyTexture(gfx->buttons[i].text_ctx.texture);
        gfx->buttons[i].text_ctx.texture = NULL;

        SDL_DestroyTexture(gfx->buttons[i].texture_ctx.image_texture);
        gfx->buttons[i].texture_ctx.image_texture = NULL;
    }

    for (size_t i = 0; i < THEMES_COUNT; i++)
    {
        Mix_FreeMusic(gfx->themes[i].music);
        gfx->themes[i].music = NULL;

        SDL_DestroyTexture(gfx->themes[i].bg.image_texture);
        gfx->themes[i].bg.image_texture = NULL;
    }

    SDL_DestroyTexture(gfx->load_screen.loading_bg.image_texture);
    gfx->load_screen.loading_bg.image_texture = NULL;

    for (size_t i = 0; i < LOAD_MESSAGES_COUNT; i++)
    {
        SDL_DestroyTexture(gfx->load_screen.messages[i].texture);
        gfx->load_screen.messages[i].texture = NULL;
    }

    SDL_DestroyTexture(gfx->load_screen.title.texture);
    gfx->load_screen.title.texture = NULL;

    SDL_FreeSurface(gfx->screen_surface);
    gfx->screen_surface = NULL;

    SDL_DestroyRenderer(gfx->rend);
    gfx->rend = NULL;

    SDL_DestroyWindow(gfx->window);
    gfx->window = NULL;

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

//——————————————————————————————————————————————————————————————————————————————————————————
