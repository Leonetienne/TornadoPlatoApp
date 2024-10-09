#pragma once
#define LARGE_RAND_DOUBLE (((rng() % 696900) - 350000) / 1000.0)
#define LARGE_RAND_POSITIVE_DOUBLE (((rng() % 350000)) / 1000.0)
#define LARGE_RAND_INT ((rng() % 6969) - 3500)
#define LARGE_RAND_POSITIVE_INT ((rng() % 3500))
