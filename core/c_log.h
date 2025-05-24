#ifndef __engine_log__
#define __engine_log__

#include <stdio.h>

// FIXME: __VA_OPT__ is not portable in C99, might be worth making these inline 
// functions?

#ifdef bse_linux
#define text_normal "\033[0m"
#define text_bold_green "\033[32;1m"
#define text_bold_yellow "\033[33;1m"
#define text_bold_red "\033[31;1m"
#define text_dark_gray "\033[90m"
#else
#define text_normal
#define text_bold_green
#define text_bold_yellow
#define text_bold_red
#define text_dark_gray
#endif

// `logDebug()` should be a no-op in release
#ifdef bse_debug
# define stringify2(x) #x
# define stringify(x) stringify2(x)
# define log_debug(...) \
  __log_msg(stderr, \
    "[" text_bold_green "debug" text_normal "] [" text_dark_gray \
    __FILE__ ":" stringify(__LINE__) text_normal "] ", \
    __VA_ARGS__)
#else
# define log_debug(...)
#endif

#define log_info(...) \
  __log_msg(stdout, \
    "[" text_dark_gray "info" text_normal "] ", \
    __VA_ARGS__)

#define log_warning(...) \
  __log_msg(stderr, \
    "[" text_bold_yellow "warning" text_normal "] ", \
    __VA_ARGS__)

#define log_error(...) \
  __log_msg(stderr, \
    "[" text_bold_red "error" text_normal "] ", \
    __VA_ARGS__)

#define log_fatal(c, ...) \
  (__log_msg( \
         stderr, \
         "[" text_bold_red "error" text_normal "] ", \
         __VA_ARGS__), exit(c))

void __log_msg(FILE* file, const char* tag, const char* fmt, ...);

#endif
