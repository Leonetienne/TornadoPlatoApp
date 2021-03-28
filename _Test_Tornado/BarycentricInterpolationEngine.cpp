#include "CppUnitTest.h"
#include "../Tornado/BarycentricInterpolationEngine.h"
#include "../_TestingUtilities/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TexturingAndStuff
{
	TEST_CLASS(_BarycentricInterpolationEngine)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_BarycentricInterpolationEngine()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that interpolation to a vertex does return that vertices value
		TEST_METHOD(Cornors_Remain_Same)
		{
			InterRenderTriangle ird;
			
			// Triangle does not need to be clipped. Is known to be all visible
			ird.c.pos_cs = { -325, 0.5, 1.1, 512 };
			ird.b.pos_cs = { 412, 0.5, 1.1, 927 };
			ird.a.pos_cs = { 0, 0.5, 1.1, 561 };

			// Perspective division
			ird.a.pos_ndc = (ird.a.pos_cs / ird.a.pos_cs.w);
			ird.b.pos_ndc = (ird.b.pos_cs / ird.b.pos_cs.w);
			ird.c.pos_ndc = (ird.c.pos_cs / ird.c.pos_cs.w);

			// Transform to screen space
			ird.a.pos_ss = ird.a.pos_ndc * 400 + Vector2d(400, 400);
			ird.b.pos_ss = ird.b.pos_ndc * 400 + Vector2d(400, 400);
			ird.c.pos_ss = ird.c.pos_ndc * 400 + Vector2d(400, 400);

			// Run interpolation test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				// Create vertex values
				double a = LARGE_RAND_DOUBLE;
				double b = LARGE_RAND_DOUBLE;
				double c = LARGE_RAND_DOUBLE;

				// Interpolate value at position of a between a, b and c
				double ret = BarycentricInterpolationEngine::PerspectiveCorrected(
					ird,
					ird.a.pos_ss,
					a,
					b,
					c
				);
				// Check the interpolated value is the exact value at a
				Assert::IsTrue(Similar(a, ret), L"Failed A");

				// Interpolate value at position of b between a, b and c
				ret = BarycentricInterpolationEngine::PerspectiveCorrected(
					ird,
					ird.b.pos_ss,
					a,
					b,
					c
				);
				// Check the interpolated value is the exact value at a
				Assert::IsTrue(Similar(b, ret), L"Failed B");

				// Interpolate value at position of b between a, b and c
				ret = BarycentricInterpolationEngine::PerspectiveCorrected(
					ird,
					ird.c.pos_ss,
					a,
					b,
					c
				);
				// Check the interpolated value is the exact value at a
				Assert::IsTrue(Similar(c, ret), L"Failed C");
			}

			return;
		}
	};
}
