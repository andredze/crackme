#define SDL_MAIN_HANDLED
#include "crack.h"
#include "graphics.h"

//------------------------------------------------------------------//

int main()
{
    AppCtx_t        app = {};
    GraphicsCtx_t   gfx = {};

    do
    {
        if (AppInit(&app, &gfx) != APP_SUCCESS)
            break;

        if (AppLoadMedia(&app, &gfx) != APP_SUCCESS)
            break;

        while (app.is_running)
        {
            if (AppUpdateState(&app, &gfx) != APP_SUCCESS)
                break;

            if (AppDraw(&app, &gfx) != APP_SUCCESS)
                break;
        }
    }
    while (0);

    AppClose(&app, &gfx);

    return EXIT_SUCCESS;
}

//----------------------------------------------------------------------
