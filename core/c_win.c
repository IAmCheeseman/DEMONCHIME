#include "c_win.h"

#include "c_engine.h"
#include "g_framebuf.h"
#include "m_math.h"
#include "w_wrap.h"

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

static void key_callback(
  GLFWwindow* window, int key, int scancode, int action, int mods)
{
  engine_t* engine = (engine_t*)glfwGetWindowUserPointer(window);

  if (engine->L == NULL) return;

  const char* key_str = get_str_from_enum(engine->L, key, key_enum);

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    lua_getglobal(engine->L, "keydown");
    if (!lua_isnil(engine->L, -1)) {
      if (!lua_isfunction(engine->L, -1)) {
        log_fatal(1, "global 'keydown' must be a function");
      }
      lua_pushstring(engine->L, key_str);
      lua_pushboolean(engine->L, action == GLFW_REPEAT);
      int res = lua_pcall(engine->L, 2, 0, engine->lua_err_handler_idx);
      if (res != LUA_OK) {
        exit(1);
      }
    }
  } else if (action == GLFW_RELEASE) {
    lua_getglobal(engine->L, "keyup");
    if (!lua_isnil(engine->L, -1)) {
      if (!lua_isfunction(engine->L, -1)) {
        log_fatal(1, "global 'keyup' must be a function");
      }
      lua_pushstring(engine->L, key_str);
      int res = lua_pcall(engine->L, 1, 0, engine->lua_err_handler_idx);
      if (res != LUA_OK) {
        exit(1);
      }
    }
  }
}

window_t window_create(engine_conf_t conf)
{
  window_t window;

  window.handle = glfwCreateWindow(
    conf.window_size.x, conf.window_size.y, conf.window_title, NULL, NULL);
  if (window.handle == NULL) {
    log_fatal(1, "could not initialize glfw window");
  }
  log_info("created glfw window");

  glfwGetWindowSize(
    window.handle,
    &window.original_size.x, &window.original_size.y);
  glfwGetWindowPos(
    window.handle,
    &window.original_pos.x, &window.original_pos.y);

  glfwMakeContextCurrent(window.handle);
  glfwSetFramebufferSizeCallback(window.handle, framebuf_resize_callback);
  glfwSetKeyCallback(window.handle, key_callback);
  glfwSwapInterval(conf.vsync);

  return window;
}

void window_destroy(window_t* window)
{
  glfwDestroyWindow(window->handle);
  window->handle = NULL;
}

void window_set_fullscreen(window_t* window, fullscreen_t fullscreen)
{
  switch (fullscreen) {
    case fullscreen_none:
      glfwSetWindowMonitor(
        window->handle, NULL,
        window->original_pos.x, window->original_pos.y,
        window->original_size.x, window->original_size.y,
        GLFW_DONT_CARE);
      break;
    case fullscreen_exclusive: {
      glfwGetWindowSize(
        window->handle,
        &window->original_size.x, &window->original_size.y);
      glfwGetWindowPos(
        window->handle,
        &window->original_pos.x, &window->original_pos.y);
      vec2i_t monitor_size;
      GLFWmonitor* primary = glfwGetPrimaryMonitor();
      glfwGetMonitorPhysicalSize(primary, &monitor_size.x, &monitor_size.y);
      glfwSetWindowMonitor(
        window->handle, primary,
        0, 0, monitor_size.x, monitor_size.y,
        GLFW_DONT_CARE);
      break;
    }
    case fullscreen_borderless:
      glfwGetWindowSize(
        window->handle,
        &window->original_size.x, &window->original_size.y);
      glfwGetWindowPos(
        window->handle,
        &window->original_pos.x, &window->original_pos.y);
      GLFWmonitor* primary = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(primary);
      glfwSetWindowMonitor(
        window->handle, primary,
        0, 0, mode->width, mode->height,
        mode->refreshRate);
      break;
  }
}

void window_swap_buffers(const window_t* window)
{
  glfwSwapBuffers(window->handle);
}

vec2i_t window_get_size(const window_t* window)
{
  vec2i_t size;
  glfwGetWindowSize(window->handle, &size.x, &size.y);
  return size;
}

void window_close(const window_t* window)
{
  glfwSetWindowShouldClose(window->handle, GLFW_TRUE);
}

bool is_window_closed(const window_t* window)
{
  return glfwWindowShouldClose(window->handle);
}
