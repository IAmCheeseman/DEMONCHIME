#ifndef __engine_engine__
#define __engine_engine__

#include <lua.h>

#include "include.h"
#include "key.h"
#include "math/vec2i.h"
#include "math/vec2f.h"
#include "timer.h"
#include "vfs.h"
#include "window.h"

typedef struct engine_s
{
  window_t window;
  lua_State* L;
  struct renderer_s* renderer;
  vfs_t* vfs;
  vfs_t* core_vfs;
  vfs_t* game_vfs;
  int lua_err_handler_idx;

  vec2i_t target_screen_size;
  struct framebuf_s* screen;
  vec2i_t screen_size;

  timer_t timer;
} engine_t;

typedef struct engine_conf_s
{
  const char* mount_path;
  const char* window_title;
  fullscreen_t fullscreen;
  vec2i_t window_size;
  vec2i_t screen_size;
  float vsync;
} engine_conf_t;

void engine_init(engine_t* engine, engine_conf_t conf);
void engine_init_lua(engine_t* engine);
void engine_destroy(engine_t* engine);
bool is_engine_init(const engine_t* engine);

void engine_update(engine_t* engine);
void engine_draw(engine_t* engine);

void engine_close(const engine_t* engine);
bool is_engine_closed(const engine_t* engine);

vec2i_t engine_get_screen_size(const engine_t* engine);

bool is_key_down(const engine_t* engine, key_t key);
bool is_mouse_down(const engine_t* engine, int btn);

vec2f_t get_mouse_pos(const engine_t* engine);

#endif
