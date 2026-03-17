#include "crack.h"
#include "graphics.h"

//----------------------------------------------------------------------

//TODO: add music | sound effects
//TODO: add images | video on bg ?
//TODO: сверять хэш файла crackme.com

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
            while (SDL_PollEvent(&app.event) != 0)
            {
                if (app.event.type == SDL_QUIT)
                {
                    app.is_running = false;
                }
            }

            if (AppDraw(&app, &gfx) != APP_SUCCESS)
                break;
        }
    }
    while (0);

    AppClose(&app, &gfx);

    return EXIT_SUCCESS;
}

//----------------------------------------------------------------------
