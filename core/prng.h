#ifndef __engine_prng__
#define __engine_prng__

#include "include.h"

typedef uint64_t Prng;

// 0 for a auto generated seed
Prng prng_create(uint32_t seed);
uint64_t prng_next(Prng* p);
double prng_nextf(Prng* p);
int prng_nexti(Prng* p);

int prng_get_rangei(Prng* p, int min, int max);
double prng_get_rangef(Prng* p, double min, double max);

#endif
