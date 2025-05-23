#ifndef __engine_prng__
#define __engine_prng__

#include "include.h"

typedef uint64_t prng_t;

// 0 for a auto generated seed
prng_t prng_create(uint32_t seed);
uint64_t prng_next(prng_t* p);
double prng_nextf(prng_t* p);
int prng_nexti(prng_t* p);

int prng_get_rangei(prng_t* p, int min, int max);
double prng_get_rangef(prng_t* p, double min, double max);

#endif
