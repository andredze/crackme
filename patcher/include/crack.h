#ifndef __CRACK_H__
#define __CRACK_H__

//------------------------------------------------------------------//

#include "graphics.h"
#include "ui_config.h"

//------------------------------------------------------------------//

const int PATCH_ADDRESS     = 0x001A;
const int TWO_NOPS_BYTECODE = 0x9090;

//------------------------------------------------------------------//

AppErr_t MakePatch(AppCtx_t* app, GraphicsCtx_t* gfx);

//------------------------------------------------------------------//

#endif /* __CRACK_H__ */
