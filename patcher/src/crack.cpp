#include "graphics.h"
#include "crack.h"

//------------------------------------------------------------------//

AppErr_t MakePatch(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    DPRINTF("Making patch\n");

    app->target_file = fopen(TARGET_FILE_NAME, "r+");

    if (app->target_file == NULL)
    {
        PRINTERR("Failed to open file %s", TARGET_FILE_NAME);
        return APP_FILE_ERROR;
    }

    DPRINTF("Opened file\n");

    if (fseek(app->target_file, PATCH_ADDRESS, SEEK_SET) != 0)
    {
        PRINTERR("fseek for PATCH_ADDRESS failed");
        return APP_FILE_ERROR;
    }

    DPRINTF("Found byte\n");

    if (fwrite(&TWO_NOPS_BYTECODE, 1, 2, app->target_file) != 2)
    {
        PRINTERR("fwrite NOPs failed");
        return APP_FILE_ERROR;
    }

    DPRINTF("Wrote nops\n");

    fclose(app->target_file);

    DPRINTF("Closed file\n");

    return APP_SUCCESS;
}

//------------------------------------------------------------------//
