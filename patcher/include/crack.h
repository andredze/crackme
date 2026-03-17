#ifndef __CRACK_H__
#define __CRACK_H__

//------------------------------------------------------------------//

#include "graphics.h"
#include "ui_config.h"

//------------------------------------------------------------------//

const int  PATCH_ADDRESS = 0x001A;
const char NOP_BYTECODE  = 0x90;

const size_t CRACKME_HASH = 14160779300616998209;
const size_t CRACKED_HASH = 12508325699750982611;

//------------------------------------------------------------------//

const char * const TARGET_FILE_NAME  = "crackme.com";
const char * const CRACKED_FILE_NAME = "cracked.com";

//------------------------------------------------------------------//

AppErr_t MakePatch(AppCtx_t* app, GraphicsCtx_t* gfx);

//------------------------------------------------------------------//

#endif /* __CRACK_H__ */
