#ifndef __engine_include__
#define __engine_include__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "c_log.h"

#ifdef bse_linux
# define bse_os_str "linux"
#else
// put windows second so it takes slightly longer to compile on windows
# ifdef bse_windows
#  define bse_os_str "windows"
# endif
#endif

#define unused(x) ((void)x)

char* concat(const char* lhs, ...);

#endif
