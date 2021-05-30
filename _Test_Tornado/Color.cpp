#include "CppUnitTest.h"
#include "../Tornado/Color.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TorGL;

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

		// Tests that if constructed with the default constructor, that all values are 0
		TEST_METHOD(Default_Constructor_All_0)
		{
			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Color color;
				Assert::AreEqual(0, (int)color.r, L"R");
				Assert::AreEqual(0, (int)color.g, L"G");
				Assert::AreEqual(0, (int)color.b, L"B");
				Assert::AreEqual(0, (int)color.a, L"A");
			}

			return;
		}

		// Tests that a color does match the values given to the constructor
		TEST_METHOD(Color_Does_Match_Constructor_Without_Alpha)
		{
			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				const double r = rng() % 255;
				const double g = rng() % 255;
				const double b = rng() % 255;

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
				const double r = rng() % 255;
				const double g = rng() % 255;
				const double b = rng() % 255;
				const double a = rng() % 255;

				Color col(r, g, b, a);
				// We could also use operator==, but this way we will know which channel failed.
				Assert::AreEqual(r, col.r);
				Assert::AreEqual(g, col.g);
				Assert::AreEqual(b, col.b);
				Assert::AreEqual(a, col.a);
			}

			return;
		}

		// Tests expected lerp result for t=0.00
		TEST_METHOD(Lerp_Expected_000)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			Assert::IsTrue(a == Color::Lerp(a, b, 0.00));

			return;
		}

		// Tests expected lerp result for t=0.25
		TEST_METHOD(Lerp_Expected_025)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			Color res = Color::Lerp(a, b, 0.25);
			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Color(25, 175, 62.5, 50) == res, wss.str().c_str());

			return;
		}

		// Tests expected lerp result for t=0.50
		TEST_METHOD(Lerp_Expected_050)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			Color res = Color::Lerp(a, b, 0.50);
			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Color(50, 150, 75, 100) == res, wss.str().c_str());

			return;
		}

		// Tests expected lerp result for t=0.75
		TEST_METHOD(Lerp_Expected_075)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			Color res = Color::Lerp(a, b, 0.75);
			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Color(75, 125, 87.5, 150) == res, wss.str().c_str());

			return;
		}

		// Tests expected lerp result for t=1.00
		TEST_METHOD(Lerp_Expected_100)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			Assert::IsTrue(b == Color::Lerp(a, b, 1.00));

			return;
		}

		// Tests lerp as a memberfunction
		TEST_METHOD(Lerp_Memberfunction)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			Assert::IsTrue(Color(75, 125, 87.5, 150) == a.Lerp(b, 0.75));
		}

		// Tests lerpSelf
		TEST_METHOD(LerpSelf)
		{
			Color a(0, 200, 50, 0);
			Color b(100, 100, 100, 200);

			a.LerpSelf(b, 0.75);

			Assert::IsTrue(Color(75, 125, 87.5, 150) == a);
		}

		// Tests that the operator equals (==) works
		TEST_METHOD(Operator_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				const double ar = rng() % 5;
				const double ag = rng() % 5;
				const double ab = rng() % 5;
				const double aa = rng() % 5;
				const double br = rng() % 5;
				const double bg = rng() % 5;
				const double bb = rng() % 5;
				const double ba = rng() % 5;

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
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				const double ar = rng() % 5;
				const double ag = rng() % 5;
				const double ab = rng() % 5;
				const double aa = rng() % 5;
				const double br = rng() % 5;
				const double bg = rng() % 5;
				const double bb = rng() % 5;
				const double ba = rng() % 5;

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
