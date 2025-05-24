#ifndef __engine_key__
#define __engine_key__

#include "include.h"

typedef enum keys_e
{
#define key_def(name, val) key_##name = (val),
#include "key_def.h"
#undef key_def
} keys_t;

#endif
