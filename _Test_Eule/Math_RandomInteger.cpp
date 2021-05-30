#include "CppUnitTest.h"
#include "../_TestingUtilities/Testutil.h"
#include "../Eule/Math.h"
#include <array>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

namespace _Math
{
	TEST_CLASS(_RandomInteger)
	{
	public:
		// Checks that the produced unsigned-integer distribution shows a big standard deviation
		TEST_METHOD(Uint_Big_Standard_Deviation)
		{
			// Setup
			std::vector<unsigned int> rands;
			rands.resize(1000);

			// Exercise
			// Create 1000 random values
			std::generate_n(rands.data(), rands.size(), []()->unsigned int { return Math::RandomUint(); });

			// Verify
			const double stddev = Testutil::Stddev<unsigned int>(rands);
			Assert::IsTrue(stddev >= 1000000, (std::wstringstream() << stddev).str().c_str());

			return;
		}

		// Checks that the produced integer distribution shows a big standard deviation
		TEST_METHOD(Int_Big_Standard_Deviation)
		{
			// Setup
			std::vector<int> rands;
			rands.resize(1000);
			
			// Exercise
			// Create 1000 random values
			std::generate_n(rands.data(), rands.size(), []()->int { return Math::RandomInt(); });

			// Verify
			const double stddev = Testutil::Stddev<int>(rands);
			Assert::IsTrue(stddev >= 1000000, (std::wstringstream() << stddev).str().c_str());

			return;
		}
	};
}
