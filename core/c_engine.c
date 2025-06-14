#include "c_engine.h"

#include <GLFW/glfw3.h>
#include <lua.h>

#include "g_renderer.h"
#include "g_framebuf.h"
#include "w_wrap.h"

void engine_init(engine_t* engine, engine_conf_t conf)
{
  log_info("initializing engine...");

  engine->vfs = NULL;

  // mount core
  vfs_err_t vfs_err = vfs_mount(&engine->vfs, "CORE.HAD");
  if (vfs_err) log_fatal(1, "could not mount 'CORE.HAD'");
  engine->core_vfs = engine->vfs;

  // mount game
  vfs_err = vfs_mount(&engine->vfs, conf.mount_path);
  if (vfs_err) log_fatal(1, "could not mount '%s'", conf.mount_path);
  engine->game_vfs = engine->vfs;

  if (glfwInit() < 0) {
    log_fatal(1, "could not initialize glfw");
  }

  engine->window = window_create(conf);

  glfwSetWindowUserPointer(engine->window.handle, engine);

  engine->renderer = NULL;
  init_renderer(engine, gfx_backend_opengl);

  engine->target_screen_size = conf.screen_size;
  engine->screen_size = conf.screen_size;
  engine->screen = framebuf_create(
    engine->renderer,
    engine->screen_size,
    framebuffer_color_buf | framebuffer_depth_buf | framebuffer_drawable
  );

  engine->timer = timer_create();
}

void engine_init_lua(engine_t* engine)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  log_info("initializing 'core'...");
  wrap(L, engine);
  if (!protected_do_file(L, engine->core_vfs, "core.lua")) {
    log_fatal(1, "was not able to initialize 'core'");
  }

  log_info("starting game...");
  if (!protected_do_file(L, engine->game_vfs, "main.lua")) {
    log_fatal(1, "was not able to run game");
  }
}

void engine_destroy(engine_t* engine)
{
  log_info("destroying engine...");

  lua_close(engine->L);

  framebuf_destroy(engine->renderer, engine->screen);

  if (engine->renderer != NULL)
    destroy_renderer(engine);

  window_destroy(&engine->window);

  vfs_destroy(engine->vfs);
  // for some reason this causes a false positive(?) memory leak with asan.
  // but i fairly sure it doesn't actually, but i'm keeping it off for debug
  // builds regardless for my own sanity
#ifndef bse_debug
  glfwTerminate();
#endif
}

bool is_engine_init(const engine_t* engine)
{
  return engine->window.handle != NULL;
}

static bool call_lua_global(const engine_t* engine, const char* fn_name)
{
  if (engine->L == NULL) return true;
  lua_getglobal(engine->L, fn_name);
  if (!lua_isnil(engine->L, -1)) {
    if (!lua_isfunction(engine->L, -1)) {
      log_error("global '%s' must be a function", fn_name);
      return false;
    }
    int res = lua_pcall(engine->L, 0, 0, engine->lua_err_handler_idx);
    return res == LUA_OK;
  }
  return true;
}

void engine_update(engine_t* engine)
{
  glfwPollEvents();

  timer_update(&engine->timer);

  if (!call_lua_global(engine, "frame")) {
    engine_close(engine);
    return;
  }

  while (timer_should_tick(&engine->timer)) {
    timer_start_tick(&engine->timer);
    if (!call_lua_global(engine, "step")) {
      engine_close(engine);
      return;
    }
  }
}

void engine_draw(engine_t* engine)
{
  engine->renderer->draw_call_count = engine->renderer->draw_call_counter;
  engine->renderer->draw_call_counter = 0;

  // Draw to screen
  framebuf_bind(engine->renderer, engine->screen);
  set_depth_test(engine->renderer, true);

  adjust_viewport(
    engine->renderer,
    (vec2f_t){engine->screen_size.x, engine->screen_size.y});

  clear_bg(engine->renderer, 0.2, 0.2, 0.2);

  if (!call_lua_global(engine, "draw")) {
    engine_close(engine);
    return;
  }

  flush_deferred(engine->renderer);

  if (!call_lua_global(engine, "uidraw")) {
    engine_close(engine);
    return;
  }

  flush_deferred(engine->renderer);

  // Draw screen
  framebuf_bind(engine->renderer, NULL);
  vec2i_t wsize = window_get_size(&engine->window);
  adjust_viewport(engine->renderer, (vec2f_t){wsize.x, wsize.y});

  clear_bg(engine->renderer, 0, 0.2, 0);
  set_depth_test(engine->renderer, false);

  framebuf_draw(
    engine->renderer,
    engine->screen,
    (vec2i_t){-1, 1}, (vec2i_t){2, -2});

  window_swap_buffers(&engine->window);

  timer_end_rendering(&engine->timer);
  //exit(1);
}


void engine_close(const engine_t* engine)
{
  window_close(&engine->window);
}

bool is_engine_closed(const engine_t* engine)
{
  return is_window_closed(&engine->window);
}

vec2i_t engine_get_screen_size(const engine_t* engine)
{
  return engine->screen_size;
}

bool is_key_down(const engine_t* engine, keys_t key)
{
  return glfwGetKey(engine->window.handle, key) == GLFW_PRESS;
}

bool is_mouse_down(const engine_t* engine, int btn)
{
  return glfwGetMouseButton(engine->window.handle, btn - 1) == GLFW_PRESS;
}

vec2f_t get_mouse_pos(const engine_t* engine)
{
  double x, y;
  glfwGetCursorPos(engine->window.handle, &x, &y);
  return (vec2f_t){x, y};
}
