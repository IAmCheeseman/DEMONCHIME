#include "c_color.h"

color_t color_from_rgba(float r, float g, float b, float a)
{
  return (color_t){r, g, b, a};
}

color_t color_from_rgb(float r, float g, float b)
{
  return (color_t){r, g, b, 1};
}

color_t color_from_rgba8(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  return (color_t){
    (float)r / UINT8_MAX,
    (float)g / UINT8_MAX,
    (float)b / UINT8_MAX,
    (float)a / UINT8_MAX,
  };
}

color_t color_from_rgb8(uint8_t r, uint8_t g, uint8_t b)
{
  return (color_t){
    (float)r / UINT8_MAX,
    (float)g / UINT8_MAX,
    (float)b / UINT8_MAX,
    1,
  };
}

void color_to_bytes(
  color_t c,
  uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a)
{
  if (r != NULL) *r = (uint8_t)(c.r * UINT8_MAX);
  if (g != NULL) *g = (uint8_t)(c.g * UINT8_MAX);
  if (b != NULL) *b = (uint8_t)(c.b * UINT8_MAX);
  if (a != NULL) *a = (uint8_t)(c.a * UINT8_MAX);
}
