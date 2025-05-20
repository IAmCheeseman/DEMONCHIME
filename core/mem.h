#ifndef __engine_mem__
#define __engine_mem__

#include "include.h"

#define GrowCapacity(cap) ((cap) == 0 ? 8 : (cap) * 2)

#ifdef bse_use_c_alloc
#define Create(size) malloc(size)
#define Realloc(ptr, size) realloc(ptr, size)
#define Destroy(ptr) free(ptr)
#else
#define Alloc(size) Allocate(NULL, size)
#define Realloc(ptr, size) Allocate(ptr, size)
#define Destroy(ptr) Allocate(ptr, 0)
#endif

void* Allocate(void* ptr, size_t size);

#endif
