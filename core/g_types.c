#include "g_types.h"

#include <string.h>

typedef struct data_type_name
{
  const char* name;
  size_t len;
  data_type_t type;
} data_type_name_t;

data_type_name_t type_names[] = {
  {"half", 4, type_half},
  {"float", 5, type_float},
  {"double", 6, type_double},
  {"uchar", 5, type_uchar},
  {"char", 4, type_char},
  {"ushort", 6, type_ushort},
  {"short", 5, type_short},
  {"uint", 4, type_uint},
  {"int", 3, type_int},
  {NULL, 0, 0},
};

size_t get_data_type_size(data_type_t type)
{
  switch (type) {
    case type_unknown: return sizeof(signed char);
    case type_half: return sizeof(float) / 2;
    case type_float: return sizeof(float);
    case type_double: return sizeof(double);
    case type_uchar: return sizeof(unsigned char);
    case type_char: return sizeof(signed char);
    case type_ushort: return sizeof(unsigned short);
    case type_short: return sizeof(signed short);
    case type_uint: return sizeof(unsigned int);
    case type_int: return sizeof(signed int);
  }
  return 0; // unreachable
}

data_type_t str_to_data_type(const char* str, size_t len)
{
  for (data_type_name_t* type = type_names; type->name != NULL; type++) {
    if (type->len == len &&
        memcmp(type->name, str, len) == 0) {
      return type->type;
    }
  }
  return type_unknown;
}
