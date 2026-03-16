#include "graphics.h"
#include "crack.h"

//----------------------------------------------------------------------

int CompareDoubles(double value1, double value2)
{
    assert(isfinite(value1));
    assert(isfinite(value2));

    return (abs(value1 - value2) < EPSILON);
}

//------------------------------------------------------------------//

static bool CursorInButton(Patcher_t* patcher)
{
    return (patcher->event.button.x >= patcher->button.x &&
            patcher->event.button.x <= patcher->button.x + patcher->button.w &&
            patcher->event.button.y >= patcher->button.y &&
            patcher->event.button.y <= patcher->button.y + patcher->button.h);
}

//------------------------------------------------------------------//

PatchErr_t PatcherUpdate(Patcher_t* patcher)
{
    assert(patcher);

    if (patcher->event.type == SDL_QUIT ||
        (patcher->event.type == SDL_KEYDOWN &&
         patcher->event.key.keysym.sym == SDLK_ESCAPE))
    {
        patcher->window_is_active = false;
    }

    if (patcher->event.type == SDL_MOUSEBUTTONDOWN &&
        patcher->state      != STATE_PATCHING      &&
        CursorInButton(patcher))
    {
        patcher->state = STATE_PATCHING;
        patcher->progress = 0.0;
        patcher->last_ticks = SDL_GetTicks();
        patcher->status_msg = "Ждеммммм...";

        if (MakePatch(patcher) != PATCH_SUCCESS)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "File Error",
                    "Patching file failed", patcher->window);
            return PATCH_FILE_ERROR;
        }
    }

    return PATCH_SUCCESS;
}

//------------------------------------------------------------------//

PatchErr_t PatcherWaitTime(Patcher_t* patcher)
{
    if (patcher->state == STATE_PATCHING)
    {
        Uint32 current_ticks = SDL_GetTicks();

        if (current_ticks - patcher->last_ticks >= PATCH_DELAY)
        {
            patcher->last_ticks = current_ticks;
            patcher->progress += 0.1;

            if (patcher->progress >= 1.0)
            {
                patcher->progress = 1.0;
                patcher->state = STATE_DONE;
                patcher->status_msg = "Готово!";
            }
        }
    }

    return PATCH_SUCCESS;
}

//------------------------------------------------------------------//

PatchErr_t PatcherCtor(Patcher_t* patcher)
{
    assert(patcher);

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    patcher->window = SDL_CreateWindow(
        WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN);

    patcher->font_big    = TTF_OpenFont(FONT_PATH, FONT_SIZE_BIG);
    patcher->font_medium = TTF_OpenFont(FONT_PATH, FONT_SIZE_MEDIUM);
    patcher->font_small  = TTF_OpenFont(FONT_PATH, FONT_SIZE_SMALL);

    if (patcher->font_big    == NULL ||
        patcher->font_medium == NULL ||
        patcher->font_small  == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Font Error",
                                "No font.ttf", patcher->window);
        SDL_Quit();
        return PATCH_NO_FONT;
    }

    patcher->renderer = SDL_CreateRenderer(
        patcher->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    patcher->window_is_active = true;
    patcher->state            = STATE_INACTIVE;
    patcher->progress         = 0.0;
    patcher->last_ticks       = 0;
    patcher->button           = {WINDOW_W / 2 - 90, 240, 180, 46};
    patcher->status_msg       = START_STATUS_MESSAGE;

    SDL_SetRenderDrawBlendMode(patcher->renderer, SDL_BLENDMODE_BLEND);

    return PATCH_SUCCESS;
}

//------------------------------------------------------------------//

void PatcherDtor(Patcher_t* patcher)
{
    TTF_CloseFont(patcher->font_big);
    TTF_CloseFont(patcher->font_medium);
    TTF_CloseFont(patcher->font_small);
    TTF_Quit();

    SDL_DestroyRenderer(patcher->renderer);
    SDL_DestroyWindow  (patcher->window);
    SDL_Quit();
}

//==================================================================//

static void RectFillColor(SDL_Renderer*    renderer,
                          SDL_Rect*        rectangle,
                          const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    SDL_RenderFillRect    (renderer, rectangle);
}

//------------------------------------------------------------------//

static void RectDrawBorder(SDL_Renderer*    renderer,
                        SDL_Rect*        rectangle,
                        const SDL_Color* color,
                        int              border_width)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);

    for (int shift = 0; shift < border_width; shift++)
    {
        SDL_Rect border_rect = {.x = rectangle->x + shift,
                                .y = rectangle->y + shift,
                                .w = rectangle->w - 2 * shift,
                                .h = rectangle->h - 2 * shift};

        SDL_RenderDrawRect(renderer, &border_rect);
    }
}

//------------------------------------------------------------------//

static void TextDraw(SDL_Renderer*    renderer,
                     TTF_Font*        font,
                     const char*      string,
                     int              x,
                     int              y,
                     const SDL_Color* color,
                     int              bg_width)
{
    if (string == NULL)
    {
        return;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, string, *color);

    if (!surface)
    {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect shift_rect = {x, y, surface->w, surface->h};

    if (bg_width > 0)
    {
        shift_rect.x = x + (bg_width - surface->w) / 2;
    }

    SDL_RenderCopy(renderer, texture, NULL, &shift_rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface   (surface);
}

//------------------------------------------------------------------//

static void DrawBackground(Patcher_t* patcher)
{
    SDL_SetRenderDrawColor(patcher->renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, COLOR_BG.a);
    SDL_RenderClear       (patcher->renderer);
}

//------------------------------------------------------------------//

static void DrawHeader(Patcher_t* patcher)
{
    SDL_Rect header = {0, 0, WINDOW_W, 58};
    RectFillColor (patcher->renderer, &header, &COLOR_HEADER);
    RectDrawBorder(patcher->renderer, &header, &COLOR_BORDER, 5);
    TextDraw      (patcher->renderer, patcher->font_big, TEXT_HEADER, 0, 13, &COLOR_TEXT, WINDOW_W);
}

//------------------------------------------------------------------//

static void DrawTargetFile(Patcher_t* patcher)
{
    char message[MAX_MESSAGE_SIZE] = {};
    snprintf(message, sizeof(message), "Ломаю: %s", TARGET_FILE);

    TextDraw(patcher->renderer, patcher->font_small,
             message, 22, 72, &COLOR_TARGET_MSG, 0);
}

//------------------------------------------------------------------//

static void DrawProgressBar(Patcher_t* patcher, SDL_Rect* bar_background)
{
    assert(patcher);
    assert(bar_background);

    *bar_background = (SDL_Rect){22, 130, WINDOW_W - 44, 26};

    RectFillColor (patcher->renderer, bar_background, &COLOR_BAR_BG);
    RectDrawBorder(patcher->renderer, bar_background, &COLOR_BORDER, 1);

    if (patcher->progress <= 0.0)
        return;

    int fill_width = (int)((bar_background->w - 2) * patcher->progress);

    if (fill_width <= 0)
    {
        return;
    }

    SDL_Rect fill_rectangle = {
        .x = bar_background->x + 1,
        .y = bar_background->y + 1,
        .w = fill_width,
        .h = bar_background->h - 2
    };

    RectFillColor(patcher->renderer, &fill_rectangle, &COLOR_BAR_FG);
}

//------------------------------------------------------------------//

static void DrawProgressText(Patcher_t* patcher, const SDL_Rect* bar_background)
{
    assert(patcher);
    assert(bar_background);

    char percent_message[MAX_MESSAGE_SIZE] = {};

    snprintf(percent_message, sizeof(percent_message),
             "%d%%", (int)(patcher->progress * 100.0));

    TextDraw(patcher->renderer, patcher->font_small,
             percent_message,
             bar_background->x,
             bar_background->y + 5,
             &COLOR_TEXT,
             bar_background->w);
}

//------------------------------------------------------------------//

static void DrawStatus(Patcher_t* patcher)
{
    assert(patcher);

    SDL_Color status_color =
        (patcher->state == STATE_ERROR) ? COLOR_ERROR :
        (patcher->state == STATE_DONE)  ? COLOR_OK :
                                          COLOR_TEXT;

    TextDraw(patcher->renderer, patcher->font_medium,
             patcher->status_msg,
             STATUS_X, STATUS_Y,
             &status_color, 0);
}

//------------------------------------------------------------------//

static void DrawPatchButton(Patcher_t* patcher, int mouse_x, int mouse_y)
{
    assert(patcher);

    int hovered = (mouse_x >= patcher->button.x &&
                   mouse_x <= patcher->button.x + patcher->button.w &&
                   mouse_y >= patcher->button.y &&
                   mouse_y <= patcher->button.y + patcher->button.h);

    SDL_Color button_color = COLOR_BUTTON_INACTIVE;

    if (patcher->state == STATE_PATCHING)
    {
        button_color = COLOR_BUTTON_PATCHING;
    }
    else if (hovered)
    {
        button_color = COLOR_BUTTON_HOVERED;
    }

    RectFillColor (patcher->renderer, &patcher->button, &button_color);
    RectDrawBorder(patcher->renderer, &patcher->button, &COLOR_BUTTON_BORDER, 1);

    const char* label = (patcher->state == STATE_PATCHING) ? "Ломаем)..." : "PATCH";

    TextDraw(patcher->renderer, patcher->font_medium,
             label,
             patcher->button.x,
             patcher->button.y + 11,
             &COLOR_TEXT,
             patcher->button.w);
}

//------------------------------------------------------------------//

PatchErr_t RenderScreen(Patcher_t* patcher)
{
    assert(patcher);

    DrawBackground(patcher);
    DrawHeader    (patcher);
    DrawTargetFile(patcher);

    SDL_Rect bar_background = {};

    DrawProgressBar (patcher, &bar_background);
    DrawProgressText(patcher, &bar_background);

    DrawStatus     (patcher);
    DrawPatchButton(patcher, patcher->mouse_x, patcher->mouse_y);

    SDL_RenderPresent(patcher->renderer);

    return PATCH_SUCCESS;
}

//----------------------------------------------------------------------
