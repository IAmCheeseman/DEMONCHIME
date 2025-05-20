#include "mem.h"

void* Allocate(void* ptr, size_t size)
{
  if (!ptr) {
    void* a = malloc(size);
    if (!a) LogFatal(1, "could not allocate");
    return a;
  }

  if (size == 0) {
    free(ptr);
    return NULL;
  }

  void* r = realloc(ptr, size);
  if (!r) LogFatal(1, "could not allocate");
  return r;
}
