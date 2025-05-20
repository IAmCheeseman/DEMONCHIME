#include "engine.h"

#include <GLFW/glfw3.h>
#include <lua.h>

#include "gfx/gfx.h"
#include "gfx/framebuffer.h"
#include "math.h"
#include "mem.h"
#include "wrap/wrap.h"

void framebuf_resize_callback(GLFWwindow* window, int width, int height)
{
  engine_t* engine = (engine_t*)glfwGetWindowUserPointer(window);

  float screen_width = engine->target_screen_size.x;
  float screen_height = engine->target_screen_size.y;

  if (width > height) { // the width should change dynamically
    float target_aspect = (float)screen_width / (float)screen_height;
    float aspect = (float)width / (float)height;
    screen_width *= aspect / target_aspect;
  } else { // the height should change dynamically
    float target_aspect = (float)screen_height / (float)screen_width;
    float aspect = (float)height / (float)width;
    screen_height *= aspect / target_aspect;
  }

  vec2i_t screen_size = (vec2i_t){ceil(screen_width), ceil(screen_height)};

  engine->screen_size = screen_size;
  framebuf_resize(engine->renderer, engine->screen, screen_size);
}

void engine_init(engine_t* engine, engine_conf_t conf)
{
  log_info("initializing engine...");

  engine->vfs = NULL;
  vfs_error_t vfs_err = vfs_mount(&engine->vfs, conf.mount_path);
  if (vfs_err) log_fatal(1, "could not mount '%s'", conf.mount_path);
  vfs_err = vfs_mount(&engine->vfs, "CORE.HAD");
  if (vfs_err) log_fatal(1, "could not mount 'CORE.HAD'");

  if (glfwInit() < 0) {
    log_fatal(1, "could not initialize glfw");
  }

  engine->window_handle = glfwCreateWindow(
    conf.window_size.x, conf.window_size.y, conf.window_title, NULL, NULL);
  if (engine->window_handle == NULL) {
    log_fatal(1, "could not initialize glfw window");
  }
  log_info("created glfw window");

  glfwMakeContextCurrent(engine->window_handle);
  glfwSetWindowUserPointer(engine->window_handle, engine);
  glfwSetFramebufferSizeCallback(
    engine->window_handle, framebuf_resize_callback);
  glfwSwapInterval(conf.vsync);

  engine->renderer = NULL;
  init_backend(engine, GFX_BACKEND_OPENGL);

  engine->target_screen_size = conf.screen_size;
  engine->screen_size = conf.screen_size;
  engine->screen = framebuf_create(
    engine->renderer,
    engine->vfs,
    engine->screen_size,
    FRAMEBUFFER_COLOR_BUF | FRAMEBUFFER_DEPTH_BUF | FRAMEBUFFER_DRAWABLE
  );


  engine->timer = timer_create();
}

void engine_init_lua(engine_t* engine)
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  log_info("initializing 'core'...");
  wrap(L, engine);
  if (!protected_do_file(L, engine, "core.lua")) {
    log_fatal(1, "was not able to initialize 'core'");
  }

  log_info("starting game...");
  if (!protected_do_file(L, engine, "main.lua")) {
    log_fatal(1, "was not able to run game");
  }
}

void engine_destroy(engine_t* engine)
{
  log_info("destroying engine...");

  lua_close(engine->L);

  framebuf_destroy(engine->renderer, engine->screen);

  if (engine->renderer != NULL)
    mem_destroy(engine->renderer);

  glfwDestroyWindow(engine->window_handle);

  vfs_destroy(engine->vfs);
  // for some reason this causes a false positive(?) memory leak with asan.
  // but i fairly sure it doesn't actually, but i'm keeping it off for debug
  // builds regardless for my own sanity
#ifndef bse_debug
  glfwTerminate();
#endif

  engine->window_handle = NULL;
}

bool is_engine_init(engine_t* engine)
{
  return engine->window_handle != NULL;
}

static bool call_lua(engine_t* engine, const char* fn_name)
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

  timer_step(&engine->timer);

  while (timer_should_tick(&engine->timer)) {
    timer_start_tick(&engine->timer);
    if (!call_lua(engine, "step")) {
      engine_close(engine);
      return;
    }
  }
}

void engine_draw(engine_t* engine)
{
  // Draw to screen
  framebuf_bind(engine->renderer, engine->screen);
  set_depth_test(engine->renderer, true);

  adjust_viewport(
    engine->renderer,
    (vec2f_t){engine->screen_size.x, engine->screen_size.y});

  clear_bg(engine->renderer, 0.2, 0.2, 0.2);

  if (!call_lua(engine, "draw")) {
    engine_close(engine);
    return;
  }

  engine_swap_buffers(engine);

  // Draw screen
  framebuf_bind(engine->renderer, NULL);
  vec2i_t wsize = engine_get_window_size(engine);
  adjust_viewport(engine->renderer, (vec2f_t){wsize.x, wsize.y});

  clear_bg(engine->renderer, 0, 0, 0);
  set_depth_test(engine->renderer, false);

  framebuf_draw(
    engine->renderer,
    engine->screen,
    (vec2i_t){-1, 1},
    (vec2i_t){2, -2});

  timer_end_rendering(&engine->timer);
  //exit(1);
}

void engine_swap_buffers(engine_t* engine)
{
  glfwSwapBuffers(engine->window_handle);
}

void engine_close(engine_t* engine)
{
  glfwSetWindowShouldClose(engine->window_handle, GLFW_TRUE);
}

bool is_engine_closed(engine_t* engine)
{
  return glfwWindowShouldClose(engine->window_handle);
}

vec2i_t engine_get_window_size(engine_t* engine)
{
  vec2i_t size;
  glfwGetWindowSize(engine->window_handle, &size.x, &size.y);
  return size;
}

vec2i_t engine_get_screen_size(engine_t* engine)
{
  return engine->screen_size;
}

bool is_key_down(engine_t* engine, key_t key)
{
  return glfwGetKey(engine->window_handle, key) == GLFW_PRESS;
}

bool is_mouse_down(engine_t* engine, int btn)
{
  return glfwGetMouseButton(engine->window_handle, btn - 1) == GLFW_PRESS;
}

vec2f_t get_mouse_pos(engine_t* engine)
{
  double x, y;
  glfwGetCursorPos(engine->window_handle, &x, &y);
  return (vec2f_t){x, y};
}
