#pragma once

/*
* Some intrinsic functions such as _mm_sincos_pd are not available on g++ by default (requires some specific library).
* So let's just "re"define them manually if we're on g++.
* This way the code still works, even with the other intrinsics enabled.
*/

#if (__GNUC__ && __cplusplus)
#include <immintrin.h>
#include <math.h>

inline __m256d _mm256_sincos_pd(__m256d* __cos, __m256d __vec)
{
	double vec[4];

	_mm256_storeu_pd(vec, __vec);

	// Manually calculate cosines
	*__cos = _mm256_set_pd(
		cos(vec[3]),
		cos(vec[2]),
		cos(vec[1]),
		cos(vec[0])
	);

	// Manually calculate sines
	return _mm256_set_pd(
		sin(vec[3]),
		sin(vec[2]),
		sin(vec[1]),
		sin(vec[0])
	);
}
#endif
