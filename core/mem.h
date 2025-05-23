#ifndef __engine_mem__
#define __engine_mem__

#include "include.h"

#define grow_capacity(cap) ((cap) == 0 ? 8 : (cap) * 2)

#ifdef bse_use_c_alloc
#define mem_alloc(size) malloc(size)
#define mem_realloc(ptr, size) realloc(ptr, size)
#define mem_destroy(ptr) free(ptr)
#else
#define mem_alloc(size) allocate(NULL, size)
#define mem_realloc(ptr, size) allocate(ptr, size)
#define mem_destroy(ptr) ((void)allocate(ptr, 0))
#endif

void* allocate(void* ptr, size_t size);

#endif
