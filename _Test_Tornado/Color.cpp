#include "CppUnitTest.h"
#include "../Tornado/Color.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TexturingAndStuff
{
	TEST_CLASS(_Color)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Color()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a color does match the values given to the constructor
		TEST_METHOD(Color_Does_Match_Constructor_Without_A)
		{
			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				const uint8_t r = rng() % 255;
				const uint8_t g = rng() % 255;
				const uint8_t b = rng() % 255;

				Color col(r, g, b);
				Assert::AreEqual(r, col.r);
				Assert::AreEqual(g, col.g);
				Assert::AreEqual(b, col.b);
				Assert::AreEqual(255, (int)col.a);
			}

			return;
		}

		// Tests that a color does match the values given to the constructor
		TEST_METHOD(Color_Does_Match_Constructor)
		{
			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				const uint8_t r = rng() % 255;
				const uint8_t g = rng() % 255;
				const uint8_t b = rng() % 255;
				const uint8_t a = rng() % 255;

				Color col(r, g, b, a);
				// We could also use operator==, but this way we will know which channel failed.
				Assert::AreEqual(r, col.r);
				Assert::AreEqual(g, col.g);
				Assert::AreEqual(b, col.b);
				Assert::AreEqual(a, col.a);
			}

			return;
		}

		// Tests that the operator equals (==) works
		TEST_METHOD(Operator_Equals)
		{
			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				const uint8_t ar = rng() % 10;
				const uint8_t ag = rng() % 10;
				const uint8_t ab = rng() % 10;
				const uint8_t aa = rng() % 10;
				const uint8_t br = rng() % 10;
				const uint8_t bg = rng() % 10;
				const uint8_t bb = rng() % 10;
				const uint8_t ba = rng() % 10;

				const Color a(ar, ag, ab, aa);
				const Color b(br, bg, bb, ba);

				Assert::AreEqual(
					(ar == br) && (ag == bg) && (ab == bb) && (aa == ba),
					a == b
				);
			}
		}

		// Tests that the operator not-equals (!=) works
		TEST_METHOD(Operator_NotEquals)
		{
			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				const uint8_t ar = rng() % 10;
				const uint8_t ag = rng() % 10;
				const uint8_t ab = rng() % 10;
				const uint8_t aa = rng() % 10;
				const uint8_t br = rng() % 10;
				const uint8_t bg = rng() % 10;
				const uint8_t bb = rng() % 10;
				const uint8_t ba = rng() % 10;

				const Color a(ar, ag, ab, aa);
				const Color b(br, bg, bb, ba);

				Assert::AreEqual(
					(ar != br) || (ag != bg) || (ab != bb) || (aa != ba),
					a != b
				);
			}
		}
		
		// Tests that the constant colors are correct
		TEST_METHOD(Constants)
		{
			Assert::IsTrue(Color(  0,   0,   0, 255) == Color::black );
			Assert::IsTrue(Color(128, 128, 128, 255) == Color::gray  );
			Assert::IsTrue(Color(255,   0,   0, 255) == Color::red	 );
			Assert::IsTrue(Color(  0, 255,   0, 255) == Color::green );
			Assert::IsTrue(Color(  0,   0, 255, 255) == Color::blue  );
			Assert::IsTrue(Color(255, 255,   0, 255) == Color::yellow);
			Assert::IsTrue(Color(  0, 255, 255, 255) == Color::cyan  );
			Assert::IsTrue(Color(255,   0, 255, 255) == Color::pink	 );
			Assert::IsTrue(Color(255, 255, 255, 255) == Color::white );
			
			return;
		}
	};
}
