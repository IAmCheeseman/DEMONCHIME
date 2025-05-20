#ifndef __engine_key__
#define __engine_key__

#include "include.h"

typedef enum key_e
{
#define KEY_DEF(name, val) KEY_##name = (val),
#include "key_def.h"
#undef KEY_DEF
} key_t;

#endif
