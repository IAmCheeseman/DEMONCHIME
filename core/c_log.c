#include "c_log.h"

#include <stdarg.h>

void __log_msg(FILE* file, const char* tag, const char* fmt, ...)
{
  if (file == NULL) file = stderr;
  if (fmt == NULL) {
    fputc('\n', file);
    return;
  }

  va_list args;
  va_start(args, fmt);
  
  if (tag != NULL) fputs(tag, file);
  vfprintf(file, fmt, args);
  fputc('\n', file);

  va_end(args);
}
