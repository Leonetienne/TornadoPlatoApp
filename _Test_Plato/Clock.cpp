#include "CppUnitTest.h"
#include "../Plato/Clock.h"
#include "../Tornado/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>
#include <thread>
#include <iomanip>
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
typedef std::chrono::milliseconds ms;

namespace Misc
{
	TEST_CLASS(_Clock)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Clock()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a new clock has as elapsed time of ~0
		TEST_METHOD(New_Clock_Elapsed_Time_0)
		{
			// Setup
			Clock clock;

			// Excersise
			const double se = clock.GetElapsedTime().AsSeconds();

			// Verify
			Assert::IsTrue(Math::Similar(se, 0));
			return;
		}

		// Tests that a clock started before a Sleep() matches the sleep duration
		TEST_METHOD(Elapsed_Time_Matches_Sleep)
		{
			// Setup
			Clock clock;

			// Excersise
			std::this_thread::sleep_for(ms(269));
			const double se = clock.GetElapsedTime().AsSeconds();

			// Verify
			// Generate debug message
			std::wstringstream wss;
			wss << "Elapsed time according to SUT: " << std::setprecision(100) << se << std::endl
			<< "Elapsed time according to calculations: " << 269e-3 << std::endl;
			Assert::IsTrue(Math::Similar(se, 269e-3, 10), wss.str().c_str());
			return;
		}

		// Tests that the elapsed time is 0 directly after resetting the clock
		TEST_METHOD(Elapsed_Time_Is_0_After_Reset)
		{
			// Setup
			Clock clock;

			// Excersise
			std::this_thread::sleep_for(ms(269));
			clock.Reset();
			const double se = clock.GetElapsedTime().AsSeconds();

			// Verify
			// Generate debug message
			std::wstringstream wss;
			wss << "Elapsed time accordint to SUT: " << std::setprecision(100) << se << std::endl;
			Assert::IsTrue(Math::Similar(se, 0), wss.str().c_str());

			return;
		}

		// Tests that the measurements between seconds, milliseconds and microseconds are accurate
		TEST_METHOD(Unit_Conversion_Works)
		{
			// Setup
			Clock clock;

			// Excersise
			std::this_thread::sleep_for(ms(2069)); // Sleep relatively long because we are going to compare SECONDS to NANOSECONDS! Yikes! We're going to need some big values for this!
			Clock::Duration dur = clock.GetElapsedTime(); // We have to cache the duration object, because fetching the individual durations would cost time itself, influencing the test result...

			const double se = dur.AsSeconds();
			const double ms = dur.AsMilliseconds();
			const double us = dur.AsMicroseconds();
			const double ns = dur.AsNanoseconds();

			// Verify
			// Generate debug message
			std::wstringstream wss;
			wss << "Elapsed time units according to SUT: " << std::setprecision(100) << std::endl
				<< "SE: " << se << std::endl
				<< "MS: " << ms << std::endl
				<< "US: " << us << std::endl
				<< "NS: " << ns << std::endl

				<< "Elapsed time units according to calculations: " << std::endl
				<< "SE: " << 2069e-3 << std::endl
				<< "MS: " << 2069e+0 << std::endl
				<< "US: " << 2069e+3 << std::endl
				<< "NS: " << 2069e+6 << std::endl

				<< std::endl;

			Assert::IsTrue(Math::Similar(2069e-3, se, 10e-3), (wss.str() + L"Failed seconds!").c_str());
			Assert::IsTrue(Math::Similar(2069e+0, ms, 10e+0), (wss.str() + L"Failed milliseconds!").c_str());
			Assert::IsTrue(Math::Similar(2069e+3, us, 10e+3), (wss.str() + L"Failed microseconds!").c_str());
			Assert::IsTrue(Math::Similar(2069e+6, ns, 10e+6), (wss.str() + L"Failed nanoseconds!").c_str());
			return;
		}
	};
}
