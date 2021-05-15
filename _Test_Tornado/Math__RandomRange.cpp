#include "CppUnitTest.h"
#include "../Tornado/Math.h"
#include <array>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace _Math
{
	TEST_CLASS(_RandomRange)
	{
	public:
		
		// Checks that a random double is never outside the specification, two positive values
		TEST_METHOD(Random_Doublerange_Never_Outside_Specification__pos__pos)
		{
			// Test 1000 random integers
			for (std::size_t i = 0; i < 1000; i++)
			{
				double rnd = Math::RandomRange(49.0, 99.0);

				Assert::IsTrue(rnd >= 49.0, L"rnd too small");
				Assert::IsTrue(rnd <= 99.0, L"rnd too big");
			}

			return;
		}

		// Checks that a random double is never outside the specification, negative minimum
		TEST_METHOD(Random_Doublerange_Never_Outside_Specification__neg__pos)
		{
			// Test 1000 random integers
			for (std::size_t i = 0; i < 1000; i++)
			{
				double rnd = Math::RandomRange(-39.0, 99.0);

				Assert::IsTrue(rnd >= -39.0, L"rnd too small");
				Assert::IsTrue(rnd <= 99.0, L"rnd too big");
			}

			return;
		}

		// Checks that a random double is never outside the specification, two negative values
		TEST_METHOD(Random_Doublerange_Never_Outside_Specification__neg__neg)
		{
			// Test 1000 random integers
			for (std::size_t i = 0; i < 1000; i++)
			{
				double rnd = Math::RandomRange(-39.0, -10.0);

				Assert::IsTrue(rnd >= -39.0, L"rnd too small");
				Assert::IsTrue(rnd <= -10.0, L"rnd too big");
			}

			return;
		}

		// Checks that the produced double-precision floating point distribution shows a big standard deviation
		TEST_METHOD(Double_Big_Standard_Deviation)
		{
			// Create 1000 random values
			std::array<double, 1000> rands;
			std::generate_n(rands.data(), rands.size(), []()->double { return Math::RandomRange(100, 4e9); });

			// Calculate mean
			double sum = 0;
			for (double& i : rands)
				sum += i;
			const double mean = sum / rands.size();

			// Calculate variance
			sum = 0;
			for (double& i : rands)
				sum += (i - mean) * (i - mean);
			const double variance = sum / (rands.size() - 1);

			// Calcuate stddev
			const double stddev = sqrt(variance);

			Assert::IsTrue(stddev >= 1000000, (std::wstringstream() << stddev).str().c_str());

			return;
		}
	};
}
