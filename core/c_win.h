#ifndef __engine_window__
#define __engine_window__

#include <GLFW/glfw3.h>

#include "include.h"
#include "m_vec2i.h"

struct engine;
struct engine_conf;

typedef enum fullscreen
{
  fullscreen_none,
  fullscreen_exclusive,
  fullscreen_borderless,
} fullscreen_t;

typedef struct window
{
  GLFWwindow* handle;
  vec2i_t original_pos;
  vec2i_t original_size;
} window_t;

window_t window_create(struct engine_conf conf);
void window_destroy(window_t* window);
void window_set_fullscreen(window_t* engine, fullscreen_t fullscreen);
void window_swap_buffers(const window_t* engine);
void window_close(const window_t* engine);
bool is_window_closed(const window_t* engine);
vec2i_t window_get_size(const window_t* engine);

#endif
