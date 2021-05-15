#include "CppUnitTest.h"
#include "../Tornado/Math.h"
#include <array>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace _Math
{
	TEST_CLASS(_RandomInteger)
	{
	public:
		// Checks that the produced unsigned-integer distribution shows a big standard deviation
		TEST_METHOD(Uint_Big_Standard_Deviation)
		{
			// Create 1000 random values
			std::array<unsigned int, 1000> rands;
			std::generate_n(rands.data(), rands.size(), []()->unsigned int { return Math::RandomUint(); });

			// Calculate mean
			double sum = 0;
			for (unsigned int& i : rands)
				sum += i;
			const double mean = sum / rands.size();

			// Calculate variance
			sum = 0;
			for (unsigned int& i : rands)
				sum += (i - mean) * (i - mean);
			const double variance = sum / (rands.size() - 1);

			// Calcuate stddev
			const double stddev = sqrt(variance);

			Assert::IsTrue(stddev >= 1000000, (std::wstringstream() << stddev).str().c_str());

			return;
		}

		// Checks that the produced integer distribution shows a big standard deviation
		TEST_METHOD(Int_Big_Standard_Deviation)
		{
			// Create 1000 random values
			std::array<int, 1000> rands;
			std::generate_n(rands.data(), rands.size(), []()->int { return Math::RandomInt(); });

			// Calculate mean
			double sum = 0;
			for (int& i : rands)
				sum += i;
			const double mean = sum / rands.size();

			// Calculate variance
			sum = 0;
			for (int& i : rands)
				sum += (i - mean) * (i - mean);
			const double variance = sum / (rands.size() - 1);

			// Calcuate stddev
			const double stddev = sqrt(variance);

			Assert::IsTrue(stddev >= 1000000, (std::wstringstream() << stddev).str().c_str());

			return;
		}
	};
}
