#include "graphics.h"
#include "crack.h"

//----------------------------------------------------------------------

PatchErr_t MakePatch(Patcher_t* patcher)
{
    assert(patcher);

    fprintf(stderr, "Making patch\n");

    patcher->fp = fopen(TARGET_FILE, "r+");

    if (patcher->fp == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", TARGET_FILE);
        return PATCH_FILE_ERROR;
    }

    fprintf(stderr, "Opened file\n");

    if (fseek(patcher->fp, PATCH_ADDRESS, SEEK_SET) != 0)
    {
        fprintf(stderr, "fseek for PATCH_ADDRESS failed\n");
        return PATCH_FILE_ERROR;
    }

    fprintf(stderr, "Found byte\n");

    if (fwrite(&TWO_NOPS_BYTECODE, 1, 2, patcher->fp) != 2)
    {
        fprintf(stderr, "fwrite NOPs failed\n");
        return PATCH_FILE_ERROR;
    }

    fprintf(stderr, "Wrote nops\n");

    fclose(patcher->fp);

    fprintf(stderr, "Closed file\n");

    return PATCH_SUCCESS;
}

//------------------------------------------------------------------//
