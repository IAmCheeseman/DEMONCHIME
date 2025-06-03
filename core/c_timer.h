#ifndef __engine_timer__
#define __engine_timer__

#include "include.h"

/// handles the fixed update loop and measures FPS and TPS
typedef struct timer
{
  double dt;
  double tick_rate;
  double accum;

  int fps;
  int tps;

  double prev_time;
  double last_fps;
  int frames_rendered;
  int ticks_ticked;
} timer_t;

timer_t timer_create();
bool timer_should_tick(const timer_t* t);
void timer_start_tick(timer_t* t);
void timer_update(timer_t* t);
void timer_end_rendering(timer_t* t);

#endif
