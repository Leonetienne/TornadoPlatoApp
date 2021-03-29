#include "CppUnitTest.h"
#include "../Tornado/InterRenderVertex.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void InitA(InterRenderVertex& a)
{
	a.pos_ws = { 0,0,0 };
	a.pos_wsmx = { 0,0,0 };
	a.pos_cs = { 0,0,0,0 };
	a.pos_ndc = { 0,0,0 };
	a.pos_ss = { 0,0,0 };
	a.pos_uv = { 0,0 };
	a.normal = { 0,0,0 };
	a.vertex_col = { 0,0,0 };

	return;
}
void InitB(InterRenderVertex& b)
{
	b.pos_ws = { 100,100,100 };
	b.pos_wsmx = { 100,100,100 };
	b.pos_cs = { 100,100,100,100 };
	b.pos_ndc = { 100,100,100 };
	b.pos_ss = { 100,100, 100 };
	b.pos_uv = { 100,100 };
	b.normal = { 100,100,100 };
	b.vertex_col = { 100,100,100 };

	return;
}

namespace Rendering
{
	TEST_CLASS(_InterRenderVertex)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_InterRenderVertex()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that nothing gets interpolated by default
		TEST_METHOD(Interpolates_Nothing_By_Default)
		{
			InterRenderVertex a;
			InterRenderVertex b;
			InitA(a);
			InitB(b);

			InterRenderVertex aold = a;

			a.Interpolate(b, 0.25);

			Assert::IsTrue(aold.pos_ws == a.pos_ws);
			Assert::IsTrue(aold.pos_cs == a.pos_cs);
			Assert::IsTrue(aold.pos_ndc == a.pos_ndc);
			Assert::IsTrue(aold.pos_ss == a.pos_ss);
			Assert::IsTrue(aold.pos_uv == a.pos_uv);
			Assert::IsTrue(aold.normal == a.normal);
			Assert::IsTrue(aold.vertex_col == a.vertex_col);

			return;
		}

		// Tests that only the correct attributes get interpolated
		TEST_METHOD(Interpolates_Correct_Attributes)
		{
			// Run test 100 times

			for (std::size_t i = 0; i < 100; i++)
			{
				// Decide that to interpolate
				const bool lerp_ws = rng() % 2;
				const bool lerp_wsmx = rng() % 2;
				const bool lerp_cs = rng() % 2;
				const bool lerp_nd = rng() % 2;
				const bool lerp_ss = rng() % 2;
				const bool lerp_uv = rng() % 2;
				const bool lerp_nm = rng() % 2;
				const bool lerp_cl = rng() % 2;

				// Generate interpolation mask
				long long mask = 0;
				if (lerp_ws)
					mask |= IRV_LERP_POS_WS;
				if (lerp_wsmx)
					mask |= IRV_LERP_POS_WSMX;
				if (lerp_cs)
					mask |= IRV_LERP_POS_CS;
				if (lerp_uv)
					mask |= IRV_LERP_POS_UV;
				if (lerp_nd)
					mask |= IRV_LERP_POS_NDC;
				if (lerp_ss)
					mask |= IRV_LERP_POS_SS;
				if (lerp_nm)
					mask |= IRV_LERP_NORMAL;
				if (lerp_cl)
					mask |= IRV_LERP_VERTEX_COLOR;

				// Create vertices
				InterRenderVertex a;
				InterRenderVertex b;
				InitA(a);
				InitB(b);
				InterRenderVertex aold = a;

				// Set interpolation mask
				a.SetInterpolationMask(mask);

				// Interpolate
				a.Interpolate(b, 0.25);

				// Check, if only the corresponding vertices have been modified
				if (lerp_ws)
					Assert::IsFalse(aold.pos_ws == a.pos_ws);
				else
					Assert::IsTrue(aold.pos_ws == a.pos_ws);

				if (lerp_wsmx)
					Assert::IsFalse(aold.pos_wsmx == a.pos_wsmx);
				else
					Assert::IsTrue(aold.pos_wsmx == a.pos_wsmx);

				if (lerp_cs)
					Assert::IsFalse(aold.pos_cs == a.pos_cs);
				else
					Assert::IsTrue(aold.pos_cs == a.pos_cs);

				if (lerp_nd)
					Assert::IsFalse(aold.pos_ndc == a.pos_ndc);
				else
					Assert::IsTrue(aold.pos_ndc == a.pos_ndc);

				if (lerp_ss)
					Assert::IsFalse(aold.pos_ss == a.pos_ss);
				else
					Assert::IsTrue(aold.pos_ss == a.pos_ss);

				if (lerp_uv)
					Assert::IsFalse(aold.pos_uv == a.pos_uv);
				else
					Assert::IsTrue(aold.pos_uv == a.pos_uv);

				if (lerp_nm)
					Assert::IsFalse(aold.normal == a.normal);
				else
					Assert::IsTrue(aold.normal == a.normal);

				if (lerp_cl)
					Assert::IsFalse(aold.vertex_col == a.vertex_col);
				else
					Assert::IsTrue(aold.vertex_col == a.vertex_col);
			}

			return;
		}
	};
}
