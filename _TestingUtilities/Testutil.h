#pragma once
#include <vector>
 
class Testutil
{
public:
    // Computes a standard derivation
	template <typename T>
	static double Stddev(const std::vector<T>& distribution)
	{
		// Calculate mean
		double sum = 0;
		for (const T& i : distribution)
			sum += i;
		const double mean = sum / distribution.size();

		// Calculate variance
		sum = 0;
		for (const T& i : distribution)
			sum += (i - mean) * (i - mean);
		const double variance = sum / (distribution.size() - 1);

		// Calcuate stddev
		const double stddev = sqrt(variance);

		return stddev;
	}
};
