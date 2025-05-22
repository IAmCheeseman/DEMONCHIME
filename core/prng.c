// based on:
// https://github.com/love2d/love/blob/main/src/modules/math/RandomGenerator.cpp
// above link has links to sources

#include "prng.h"

#include <time.h>

static uint64_t wang_hash_64(uint64_t key)
{
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	key = key ^ (key >> 28);
	key = key + (key << 31);
	return key;
}

Prng prng_create(uint32_t seed)
{
  if (seed == 0) {
    seed = time(NULL);
  }
  return wang_hash_64(seed);
}

uint64_t prng_next(Prng* p)
{
  *p ^= (*p >> 12);
	*p ^= (*p << 25);
	*p ^= (*p >> 27);
  return *p * 2685821657736338717ULL;
}

double prng_nextf(Prng* p)
{
  uint64_t r = prng_next(p);
  union {
    uint64_t uint64;
    double d;
  } n;
  n.uint64 = ((0x3FFULL) << 52) | (r >> 12);;
  return n.d - 1.0;
}

int prng_nexti(Prng* p)
{
  union {
    uint64_t uint64;
    int i;
  } n;
  n.uint64 = prng_next(p);
  return n.i;
}

int prng_get_rangei(Prng* p, int min, int max)
{
  int i = prng_nexti(p);
  return i % (max - min + 1) + min;
}

double prng_get_rangef(Prng* p, double min, double max)
{
  double d = prng_nextf(p);
  return d * (max - min) + min;
}
