#ifndef __engine_l_level
#define __engine_l_level

#include "m_math.h"
#include "c_color.h"
#include "g_mesh.h"
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

  tex_t* tex;
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

typedef struct level_vertex
{
  vec3f_t pos;
  vec3f_t normal;
  vec2f_t uv;
  color_t color;
} level_vertex_t;

typedef struct level_model
{
  mesh_t mesh;
  tex_t* tex;

  level_vertex_t* vertices;
  size_t vertices_len;
} level_model_t;

typedef struct level
{
  const char* title;

  sector_t* sectors;
  size_t sectors_len;

  tex_t* texs;
  size_t texs_len;

  level_model_t* models;
  size_t models_len;
} level_t;

// binary spec for levels:
//
// UBYTE - u8
// BYTE - s8
// USHORT - u16
// SHORT - s16
// UINT - u32
// INT - s32 
// FLOAT - f32
// DOUBLE - f64
// T[n] - array of type T n bytes long
// ARR:T - length: UINT, arr: T[length]
// ARR:{*} - length: UINT, arr: *[length]
//
// header:
// UINT - file size
// UBYTE[10] - magic number. should be "DEMONCHIME"
// ARR:UBYTE - level title
// UINT - offset from the beginning of the file to the sector section
// UINT - offset from the beginning of the file to the lines section
// UINT - offset from the beginning of the file to the model section
// UINT - offset from the beginning of the file to the texture section
//
// sector section:
// UINT - number of sectors
// ARR:{
//   ARR:UINT - lines in this sector
//   FLOAT - floor height
//   FLOAT - ceiling height. base is floor height
// }
//
// line section:
// ARR:{
//   UINT - offset for line touching where this line starts
//   UINT - offset for line touching where this line ends
//   FLOAT - start x position
//   FLOAT - start y position
//   FLOAT - end x position
//   FLOAT - end y position
//   FLOAT - normal x
//   FLOAT - normal y
//   UINT - texture offset
//   UBYTE - double sided. boolean
// }
//
// model section:
// ARR:{
//   UINT - texture offset
//   ARR:{
//     FLOAT - x
//     FLOAT - y
//     FLOAT - z
//
//     FLOAT - normal x
//     FLOAT - normal y
//     FLOAT - normal z
//
//     FLOAT - u
//     FLOAT - v
//
//     FLOAT - r
//     FLOAT - g
//     FLOAT - b
//   }
// }
//
// texture section:
// ARR:{
//   ARR:UBYTE - path to the texture
// }

void load_binary(level_t* l, uint8_t* b);
void load_binary_from_vfs(level_t* l, vfs_t* vfs, const char* path);
uint8_t* dump_to_binary(const level_t* l);

void level_gen_meshes(level_t* l);

#endif
