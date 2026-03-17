#include "graphics.h"
#include "crack.h"

//------------------------------------------------------------------//

size_t CountHash(char* data, size_t size)
{
    assert(data);

    size_t hash = 5381;

    for (size_t i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + data[i];
    }

    return hash;
}

//------------------------------------------------------------------//

AppErr_t MakePatch(AppCtx_t* app, GraphicsCtx_t* gfx)
{
    assert(app);
    assert(gfx);

    DPRINTF("Making patch\n");

    app->crack_input = {.input_file_info  = {.filepath = TARGET_FILE_NAME},
                        .output_file_info = {.filepath = CRACKED_FILE_NAME}};

    if (OpenFile(&app->crack_input.input_file_info, "rb"))
    {
        return APP_FILE_ERROR;
    }
    if (OpenFile(&app->crack_input.output_file_info, "wb"))
    {
        fclose(app->crack_input.input_file_info.stream);
        return APP_FILE_ERROR;
    }

    DPRINTF("Opened file\n");

    if (ReadText(&app->crack_input))
    {
        fclose(app->crack_input.input_file_info.stream);
        fclose(app->crack_input.output_file_info.stream);
        return APP_FILE_ERROR;
    }

    if (CRACKME_HASH != CountHash(app->crack_input.buffer_data.buffer,
                                     app->crack_input.input_file_info.size))
    {
        PRINTERR("This file is not crackme.com that I'm hacking");

        free(app->crack_input.buffer_data.buffer);
        fclose(app->crack_input.input_file_info.stream);
        fclose(app->crack_input.output_file_info.stream);

        return APP_WRONG_FILE;
    }

    printf("hash = %zu\n", CountHash(app->crack_input.buffer_data.buffer,
                                     app->crack_input.input_file_info.size));

    app->crack_input.buffer_data.buffer[PATCH_ADDRESS]     = NOP_BYTECODE;
    app->crack_input.buffer_data.buffer[PATCH_ADDRESS + 1] = NOP_BYTECODE;

    DPRINTF("Wrote nops\n");
    printf("cracked hash = %zu\n", CountHash(app->crack_input.buffer_data.buffer,
                                             app->crack_input.input_file_info.size));

    if (fwrite(app->crack_input.buffer_data.buffer,
               app->crack_input.input_file_info.size,
               1,
               app->crack_input.output_file_info.stream) != 1)
    {
        PRINTERR("fwrite to %s failed", app->crack_input.output_file_info.filepath);
        free(app->crack_input.buffer_data.buffer);
        fclose(app->crack_input.input_file_info.stream);
        fclose(app->crack_input.output_file_info.stream);
        return APP_FILE_ERROR;
    }

    free(app->crack_input.buffer_data.buffer);
    fclose(app->crack_input.input_file_info.stream);
    fclose(app->crack_input.output_file_info.stream);

    DPRINTF("Closed files\n");

    return APP_SUCCESS;
}

//------------------------------------------------------------------//
