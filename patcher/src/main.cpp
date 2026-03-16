#include "crack.h"
#include "graphics.h"

//----------------------------------------------------------------------

int main()
{
    Patcher_t patcher = {};

    if (PatcherCtor(&patcher) != PATCH_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    while (patcher.window_is_active)
    {
        SDL_GetMouseState(&patcher.mouse_x, &patcher.mouse_y);

        while (SDL_PollEvent(&patcher.event))
        {
            PatcherUpdate(&patcher);
        }

        PatcherWaitTime(&patcher);
        RenderScreen   (&patcher);
    }

    PatcherDtor(&patcher);
}

//----------------------------------------------------------------------
