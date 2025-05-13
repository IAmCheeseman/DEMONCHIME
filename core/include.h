#ifndef __engine_include__
#define __engine_include__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "log.h"

#ifdef bse_linux
# define bse_os_str "linux"
#else
// put windows second so it takes slightly longer to compile on windows
# ifdef bse_windows
#  define bse_os_str "windows"
# endif
#endif

// TODO: Temp, replace with a vfs
char* ReadFile(const char* path);

#endif
