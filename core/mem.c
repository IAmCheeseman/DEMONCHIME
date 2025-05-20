#include "mem.h"

void* allocate(void* ptr, size_t size)
{
  if (ptr && size == 0) {
    free(ptr);
    return NULL;
  }

  void* r = realloc(ptr, size);
  if (!r) log_fatal(1, "could not allocate");
  return r;
}
