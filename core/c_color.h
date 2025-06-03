#ifndef __engine_color__
#define __engine_color__

#include "include.h"

typedef struct color
{
  float r;
  float g;
  float b;
  float a;
} color_t;

color_t color_from_rgba(float r, float g, float b, float a);
color_t color_from_rgb(float r, float g, float b);

color_t color_from_rgba8(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
color_t color_from_rgb8(uint8_t r, uint8_t g, uint8_t b);

void color_to_bytes(color_t c, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);

#endif
