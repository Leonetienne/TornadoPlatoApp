#include "CppUnitTest.h"
#include "../_TestingUtilities/Testutil.h"
#include "../Eule/Math.h"
#include <array>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

namespace _Math
{
	TEST_CLASS(_RandomIntRange)
	{
	public:
		// Checks that a random integer is never outside the specification, two positive values
		TEST_METHOD(Never_Outside_Specification__pos__pos)
		{
			// Test 1000 random integers
			for (std::size_t i = 0; i < 1000; i++)
			{
				int rnd = Math::RandomIntRange(49, 99);

				Assert::IsTrue(rnd >= 49, L"rnd too small");
				Assert::IsTrue(rnd <= 99, L"rnd too big");
			}

			return;
		}

		// Checks that a random integer is never outside the specification, negative minimum
		TEST_METHOD(Never_Outside_Specification__neg__pos)
		{
			// Test 1000 random integers
			for (std::size_t i = 0; i < 1000; i++)
			{
				int rnd = Math::RandomIntRange(-39, 99);

				Assert::IsTrue(rnd >= -39, L"rnd too small");
				Assert::IsTrue(rnd <= 99, L"rnd too big");
			}

			return;
		}

		// Checks that a random integer is never outside the specification, two negative values
		TEST_METHOD(Never_Outside_Specification__neg__neg)
		{
			// Test 1000 random integers
			for (std::size_t i = 0; i < 1000; i++)
			{
				int rnd = Math::RandomIntRange(-39, -10);

				Assert::IsTrue(rnd >= -39, L"rnd too small");
				Assert::IsTrue(rnd <= -10, L"rnd too big");
			}

			return;
		}

		// Checks that the random intrange method also returns the supplied limits
		TEST_METHOD(Inclusivity)
		{
			// Test 1000 random integers
			// The chance that any number [0,9] will not drop at least once is basically 0

			std::array<bool, 10> foundDigits;
			foundDigits.fill(false);

			for (std::size_t i = 0; i < 1000; i++)
			{
				int randomVal = Math::RandomIntRange(0, 9);
				foundDigits[randomVal] = true;
			}

			// Check that each value has been rolled at least once
			for (const bool& b : foundDigits)
				Assert::IsTrue(b);

			return;
		}

		// Checks that the produced integer distribution shows a big standard deviation
		TEST_METHOD(Big_Standard_Deviation)
		{
			// Setup
			std::vector<int> rands;
			rands.resize(1000);

			// Exercise
			// Create 1000 random values
			std::generate_n(rands.data(), rands.size(), []()->int { return Math::RandomIntRange(100, (int)4e9); });
			
			// Verify
			const double stddev = Testutil::Stddev(rands);
			Assert::IsTrue(stddev >= 1000000, (std::wstringstream() << stddev).str().c_str());

			return;
		}
	};
}
