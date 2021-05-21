#include "CppUnitTest.h"
#include "Testutil.h"
#include "../Tornado/Math.h"
#include "../Tornado/Constants.h"
#include <array>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace _Math
{
	TEST_CLASS(_Oscillate)
	{
	public:

		// Checks that an oscillation of speed 1 between -1 and 1 is just equal to sin(counter*pi-pi/2)
		TEST_METHOD(Oracle_Sin)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double rnd = Math::RandomRange(-1000, 1000);
				
				// Exercise
				const double result = Math::Oscillate(-1, 1, rnd, 1);

				// Verify
				const double expected = sin(rnd * PI - HALF_PI);
				Assert::IsTrue(Math::Similar(expected, result));
			}

			return;
		}

		// Tests that the result is a, if the counter is 0 or a whole, even integer
		TEST_METHOD(Returns_a_For_Counter_0)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1000, 1000);
				const double b = Math::RandomRange(-1000, 1000);
				const int even = Math::RandomIntRange(-1000, 1000) & ~1;

				// Exercise
				const double result = Math::Oscillate(a, b, even, 1);

				// Verify
				const double expected = a;

				std::wstringstream wss;
				wss << std::endl
					<< "a: " << a << std::endl
					<< "b: " << b << std::endl
					<< "expected: " << expected << std::endl
					<< "result: " << result << std::endl
				<< std::endl;

				Assert::IsTrue(Math::Similar(expected, result), wss.str().c_str());
			}
		}

		// Tests that the result is b, if the counter is a whole, uneven integer
		TEST_METHOD(Returns_b_For_Uneven_Whole_Counter)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1000, 1000);
				const double b = Math::RandomRange(-1000, 1000);
				const int uneven = Math::RandomIntRange(-1000, 1000) | 1;

				// Exercise
				const double result = Math::Oscillate(a, b, uneven, 1);

				// Verify
				const double expected = b;
				Assert::IsTrue(Math::Similar(expected, result));
			}
		}

		// Tests that the result is (a+b)/2, when counter satisfies (int)x + 0.5
		TEST_METHOD(Returns_ab_mean_for_intx_plus_0p5)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1000, 1000);
				const double b = Math::RandomRange(-1000, 1000);
				const int anInt = Math::RandomIntRange(-1000, 1000);

				// Exercise
				const double result = Math::Oscillate(a, b, anInt + 0.5, 1);

				// Verify
				const double expected = (a+b) / 2.0;

				std::wstringstream wss;
				wss << std::endl
					<< "a: " << a << std::endl
					<< "b: " << b << std::endl
					<< "expected: " << expected << std::endl
					<< "result: " << result << std::endl
					<< std::endl;
				Assert::IsTrue(Math::Similar(expected, result), wss.str().c_str());
			}
		}

		// Tests that the result is (3a+b)/4, when counter satisfies 2(int)x + 0.25
		TEST_METHOD(Returns_3ab_mean_for_intx_plus_0p25_counterbase_even)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1, 1);
				const double b = Math::RandomRange(-1, 1);
				const int even = Math::RandomIntRange(-1000, 1000) & ~1;

				// Exercise
				const double result = Math::Oscillate(a, b, even + 0.25, 1);

				// Verify
				const double expected = (3*a + b) / 4.0;

				std::wstringstream wss;
				wss << std::endl
					<< "a: " << a << std::endl
					<< "b: " << b << std::endl
					<< "expected: " << expected << std::endl
					<< "result: " << result << std::endl
					<< std::endl;

				// Oscillate is not linear, we just want a really rough approximation
				Assert::IsTrue(Math::Similar(expected, result, 0.4), wss.str().c_str());
			}
		}

		// Tests that the result is (a+3b)/4, when counter satisfies 2(int)x + 0.75
		TEST_METHOD(Returns_a3b_mean_for_intx_plus_0p75_counterbase_even)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1, 1);
				const double b = Math::RandomRange(-1, 1);
				const int even = Math::RandomIntRange(-1000, 1000) & ~1;

				// Exercise
				const double result = Math::Oscillate(a, b, even + 0.75, 1);

				// Verify
				const double expected = (a + 3*b) / 4.0;

				// Oscillate is not linear, we just want a really rough approximation
				Assert::IsTrue(Math::Similar(expected, result, 0.4)); // Oscillate is not linear
			}
		}

		// Tests that the result is (a+3b)/4, when counter satisfies 2(int)x+1 + 0.25
		TEST_METHOD(Returns_3ab_mean_for_intx_plus_0p25_counterbase_uneven)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1, 1);
				const double b = Math::RandomRange(-1, 1);
				const int uneven = Math::RandomIntRange(-1000, 1000) | 1;

				// Exercise
				const double result = Math::Oscillate(a, b, uneven + 0.25, 1);

				// Verify
				const double expected = (a + 3*b) / 4.0;

				// Oscillate is not linear, we just want a really rough approximation
				Assert::IsTrue(Math::Similar(expected, result, 0.4));
			}
		}

		// Tests that the result is (3a+b)/4, when counter satisfies 2(int)x+1 + 0.75
		TEST_METHOD(Returns_a3b_mean_for_intx_plus_0p75_counterbase_uneven)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1, 1);
				const double b = Math::RandomRange(-1, 1);
				const int uneven = Math::RandomIntRange(-1000, 1000) | 1;

				// Exercise
				const double result = Math::Oscillate(a, b, uneven + 0.75, 1);

				// Verify
				const double expected = (3*a + b) / 4.0;

				// Oscillate is not linear, we just want a really rough approximation
				Assert::IsTrue(Math::Similar(expected, result, 0.4)); // Oscillate is not linear
			}
		}

		// Tests that doubling the speed will double the frequency
		TEST_METHOD(Doubling_Speed_Doubles_Frequency)
		{
			// Test 1000 random floats
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				const double a = Math::RandomRange(-1000, 1000);
				const double b = Math::RandomRange(-1000, 1000);

				// Exercise
				const double result = Math::Oscillate(a, b, 0.5, 2);

				// Verify
				const double expected = b;
				Assert::IsTrue(Math::Similar(expected, result));
			}

			return;
		}
	};
}
