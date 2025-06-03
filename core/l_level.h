#ifndef __engine_l_level
#define __engine_l_level

#include "m_math.h"
#include "g_tex.h"

typedef struct line
{
  // if a line starts or ends where this one starts, a reference will be here
  struct line* prev; 
  // if a line starts or ends where this one ends, a reference will be here
  struct line* next;

  vec2f_t start;
  vec2f_t end;

  size_t tex_id;

  bool double_sided;
  vec2f_t normal;
} line_t;

typedef struct sector
{
  // lines are organized clock-wise
  line_t* lines;
  size_t lines_len;

  float floor_z;
  float h; // floor_z + h = ceiling_z
} sector_t;

typedef struct level
{
  const char* title;

  sector_t* sectors;
  size_t sectors_len;

  tex_t* texs;
  size_t texs_len;
} level_t;

#endif
