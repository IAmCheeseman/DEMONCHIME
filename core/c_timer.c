#include "c_timer.h"

#include <GLFW/glfw3.h>

timer_t timer_create()
{
  timer_t t;
  t.tick_rate = 30;
  t.accum = 0;
  t.fps = 0;
  t.tps = 0;
  t.prev_time = 0;
  t.last_fps = 0;
  t.frames_rendered = 0;
  t.ticks_ticked = 0;
  return t;
}

bool timer_should_tick(const timer_t* t)
{
  return t->accum > 1.0 / t->tick_rate;
}

void timer_start_tick(timer_t* t)
{
  t->accum -= 1.0 / t->tick_rate;
  if (t->accum < 0) t->accum = 0;
  t->ticks_ticked++;
}

void timer_update(timer_t* t)
{
  double current_time = glfwGetTime();
  t->dt = current_time - t->prev_time;
  t->prev_time = current_time;

  t->accum += t->dt;

  double since_fps_update = current_time - t->last_fps;
  if (since_fps_update > 1) {
    t->fps = (double)t->frames_rendered / since_fps_update;
    t->tps = (double)t->ticks_ticked / since_fps_update;
    t->last_fps = current_time;

    t->frames_rendered = 0;
    t->ticks_ticked = 0;
  }
}

void timer_end_rendering(timer_t* t)
{
  t->frames_rendered++;
}
