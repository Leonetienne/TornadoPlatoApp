#include "CppUnitTest.h"
#include "../Tornado/ClippingEngine.h"
#include "../Eule/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TorGL;

// HomoDot method to check if clipped triangles are indeed no longer outside the clipping space
double HomoDot(uint8_t edge, const Vector4d& v)
{
	switch (edge) {
	case 0: // left
		return -v.x + v.w;
	case 1: // right
		return v.x + v.w;
	case 2: // top
		return -v.y + v.w;
	case 3: // bottom
		return v.y + v.w;
	case 4: // near
		return v.z;
	case 5: // far
		return -v.z + v.w;
	}

	return 0.0; // Should not come to this point
}

// Outcode method a litle more forgiving, to adjust for floating point inaccuracy
uint8_t Outcode(const Vector4d& v)
{
	uint8_t outcode = 0;

	for (uint8_t i = 0; i < 6; i++)
	{
		// The HomoDot() result would be most likely <-100, if failed
		// So, allowing x >= -0.00001 to verify should be okay :3
		if (HomoDot(i, v) < -0.00001)
		{
			outcode |= (1 << i);
		}
	}

	return outcode;
}

namespace Engines
{
	TEST_CLASS(_ClippingEngine)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_ClippingEngine()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a completely visible triangle is not clipped
		TEST_METHOD(Does_Not_Clip_Completely_Visibe_Triangle)
		{
			// Create triangle that is knowingly all inside the clipping space
			InterRenderTriangle ird;
			ird.a.pos_cs = { -325, 0.5, 1.1, 512 };
			ird.b.pos_cs = {  412, 0.5, 1.1, 927 };
			ird.c.pos_cs = { 0, 0.5, 1.1, 561 };

			std::vector<InterRenderTriangle> ret = ClippingEngine::Clip(ird);

			Assert::IsTrue(1 == ret.size(), L"Clip returned the wrong number of triangles!");

			InterRenderTriangle tri = ret[0];

			Assert::IsTrue(tri.a.pos_cs == ird.a.pos_cs, L"A-Vertex failed");
			Assert::IsTrue(tri.b.pos_cs == ird.b.pos_cs, L"B-Vertex failed");
			Assert::IsTrue(tri.c.pos_cs == ird.c.pos_cs, L"C-Vertex failed");

			return;
		}

		// Tests that a completely visible triangle is completely dropped
		TEST_METHOD(Drops_Completely_Invisible_Triangle)
		{
			// Create triangle that is knowingly all outside the clipping space

			// This triangle looks somewhat like this in clipping space (frontal view):
			// Notice how it does not meet the criteria for a fast-reject? Not all vertices violate a the same edge.
			//                         A            B
			// 
			//  +---------------------------+
			//  |                           |
			//  |                           |       C
			//  |                           |
			//  |                           |
			//  |                           |
			//  |                           |
			//  |                           |
			//  |                           |
			//  +---------------------------+


			InterRenderTriangle ird;
			ird.a.pos_cs = { 273.5, -820.5, 1.1, 547 };
			ird.b.pos_cs = { 718.5, -718.5, 3.55, 479 };
			ird.c.pos_cs = { 918, -459, 9.1, 612 };

			std::vector<InterRenderTriangle> ret = ClippingEngine::Clip(ird);

			Assert::IsTrue(0 == ret.size(), L"Clip returned the wrong number of triangles!");

			return;
		}

		// Tests that a completely visible triangle is not clipped. Should trigger fast-reject
		TEST_METHOD(Drops_Completely_Invisible_riangle__TriggerFastReject)
		{
			// Create triangle that is knowingly all outside the clipping space

			// This triangle looks somewhat like this in clipping space (frontal view):
			// Notice how it does meet the criteria for a fast-reject? All vertices violate a the same edge. 
			//  
			// 
			//                 +---------------------------+
			//                 |                           |
			//                 |                           |  
			//  C          A   |                           |
			//                 |                           |
			//                 |                           |
			//        B        |                           |
			//                 |                           |
			//                 |                           |
			//                 +---------------------------+

			InterRenderTriangle ird;
			ird.a.pos_cs = { -325, 0.5, 1.1, 2 };
			ird.b.pos_cs = { -412, -0.5, 1.1, 14 };
			ird.c.pos_cs = { -1000, 0.5, 1.1, 3 };

			std::vector<InterRenderTriangle> ret = ClippingEngine::Clip(ird);

			Assert::IsTrue(0 == ret.size(), L"Clip returned the wrong number of triangles!");

			return;
		}

		// Clips with randomized vertices and tests that no verturned vertex is outside the boundries
		TEST_METHOD(RandomClip_No_Vertex_Outside_Clipping_Space)
		{
			// Run test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				InterRenderTriangle ird;
				#define rnddnum rng() % 1000
				ird.a.pos_cs = Vector4d(rnddnum - 500, rnddnum - 500, rnddnum - 500, rng() % 500 + 25.0);
				ird.b.pos_cs = Vector4d(rnddnum - 500, rnddnum - 500, rnddnum - 500, rng() % 500 + 25.0);
				ird.c.pos_cs = Vector4d(rnddnum - 500, rnddnum - 500, rnddnum - 500, rng() % 500 + 25.0);
				#undef rnddnum

				std::wstringstream wss;
				{
					InterRenderTriangle test = ird;
					test.a.pos_cs /= test.a.pos_cs.w;
					test.b.pos_cs /= test.b.pos_cs.w;
					test.c.pos_cs /= test.c.pos_cs.w;

					wss << std::endl
						<< "Initial triangle:" << std::endl
						<< "a: " << test.a.pos_cs << std::endl
						<< "b: " << test.b.pos_cs << std::endl
						<< "c: " << test.c.pos_cs << std::endl
						<< std::endl;
				}


				std::vector<InterRenderTriangle> ret = ClippingEngine::Clip(ird);

				int counter = 0;
				for (InterRenderTriangle& ir : ret)
				{
					uint8_t out_a = Outcode(ir.a.pos_cs);
					uint8_t out_b = Outcode(ir.b.pos_cs);
					uint8_t out_c = Outcode(ir.c.pos_cs);

					// Do perspective divide
					ir.a.pos_cs /= ir.a.pos_cs.w;
					ir.b.pos_cs /= ir.b.pos_cs.w;
					ir.c.pos_cs /= ir.c.pos_cs.w;

					wss << std::endl
						<< "i = " << counter << std::endl
						<< "sz = " << ret.size() << std::endl
						<< "a: " << ir.a.pos_cs << std::endl
						<< "b: " << ir.b.pos_cs << std::endl
						<< "c: " << ir.c.pos_cs << std::endl
						<< std::endl;

					if ((out_a | out_b | out_c) != 0)
						Assert::Fail(wss.str().c_str());
					
					counter++;
				}
			}

			return;
		}

		// Tests that if only the A vertex is inside, the vertex order will stay intact
		TEST_METHOD(Vertex_Order_Stays_Intact_1_In_A)
		{
			InterRenderTriangle ird;
			ird.a.pos_cs = { 0, 0, 4, 547 };
			ird.b.pos_cs = { -600, 100, 4, 479 };
			ird.c.pos_cs = { -600, -100, 4, 612 };

			InterRenderTriangle ret = ClippingEngine::Clip(ird)[0];

			std::wstringstream wss;
			wss << std::endl
				<< "a:" << ret.a.pos_cs << std::endl
				<< "b:" << ret.b.pos_cs << std::endl
				<< "c:" << ret.c.pos_cs << std::endl
				<< std::endl;

			Assert::IsTrue(ret.a.pos_cs == ird.a.pos_cs,    (wss.str() + L"A").c_str());  // a should stay untouched
			Assert::IsTrue(ret.b.pos_cs.y > ret.c.pos_cs.y, (wss.str() + L"BC").c_str()); // b.y should still be above c.y

			return;
		}

		// Tests that if only the B vertex is inside, it will not be modified
		TEST_METHOD(Vertex_Order_Stays_Intact_1_In_B)
		{
			InterRenderTriangle ird;
			ird.a.pos_cs = { -600, 100, 4, 479 };
			ird.b.pos_cs = { 0, 0, 4, 547 };
			ird.c.pos_cs = { -600, -100, 4, 612 };

			InterRenderTriangle ret = ClippingEngine::Clip(ird)[0];

			std::wstringstream wss;
			wss << std::endl
				<< "a:" << ret.a.pos_cs << std::endl
				<< "b:" << ret.b.pos_cs << std::endl
				<< "c:" << ret.c.pos_cs << std::endl
				<< std::endl;

			Assert::IsTrue(ret.b.pos_cs == ird.b.pos_cs,	  (wss.str() + L"B").c_str());  // b should stay untouched
			Assert::IsTrue(ret.a.pos_cs.y > ret.c.pos_cs.y, (wss.str() + L"AC").c_str()); // a.y should still be above c.y

			return;
		}

		// Tests that if only the C vertex is inside, it will not be modified
		TEST_METHOD(Vertex_Order_Stays_Intact_1_In_C)
		{
			InterRenderTriangle ird;
			ird.a.pos_cs = { -600, 100, 4, 479 };
			ird.b.pos_cs = { -600, -100, 4, 612 };
			ird.c.pos_cs = { 0, 0, 4, 547 };

			InterRenderTriangle ret = ClippingEngine::Clip(ird)[0];

			std::wstringstream wss;
			wss << std::endl
				<< "a:" << ret.a.pos_cs << std::endl
				<< "b:" << ret.b.pos_cs << std::endl
				<< "c:" << ret.c.pos_cs << std::endl
				<< std::endl;

			Assert::IsTrue(ret.c.pos_cs == ird.c.pos_cs,    (wss.str() + L"C").c_str());  // c should stay untouched
			Assert::IsTrue(ret.a.pos_cs.y > ret.b.pos_cs.y, (wss.str() + L"AB").c_str()); // a.y should still be above b.y

			return;
		}

		// Tests that if the A and B vertices are inside, the vertex order will stay intact
		TEST_METHOD(Vertex_Order_Stays_Intact_2_In_AB)
		{
			InterRenderTriangle ird;
			ird.a.pos_cs = { 0, 100, 4, 547 };
			ird.b.pos_cs = { 0, -100, 4, 547 };
			ird.c.pos_cs = { -900, 0, 4, 612 };

			std::vector<InterRenderTriangle> res = ClippingEngine::Clip(ird);
			Assert::AreEqual(2, (int)res.size(), L"Triangle return count mismatch");
			InterRenderTriangle ret1 = res[0];
			InterRenderTriangle ret2 = res[1];

			std::wstringstream wss;
			wss << std::endl
				<< "a1:" << ret1.a.pos_cs << std::endl
				<< "b1:" << ret1.b.pos_cs << std::endl
				<< "c1:" << ret1.c.pos_cs << std::endl
				<< std::endl
				<< "a2:" << ret2.a.pos_cs << std::endl
				<< "b2:" << ret2.b.pos_cs << std::endl
				<< "c2:" << ret2.c.pos_cs << std::endl
				<< std::endl;

			Assert::IsTrue(ret1.a.pos_cs == ird.a.pos_cs, (wss.str() + L"A1").c_str());    // a1 should stay untouched
			Assert::IsTrue(ret1.b.pos_cs == ird.b.pos_cs, (wss.str() + L"B1").c_str());    // b1 should stay untouched
			Assert::IsTrue(ret2.b.pos_cs == ird.b.pos_cs, (wss.str() + L"B2").c_str());    // b2 should stay untouched
			Assert::IsTrue(ret1.c.pos_cs.x < ret1.a.pos_cs.x, (wss.str() + L"C1<B1").c_str());	 // c1.x should still be to the left of a1.x
			Assert::IsTrue(ret2.c.pos_cs.x < ret2.b.pos_cs.x, (wss.str() + L"C2<B2").c_str());	 // c2.x should still be to the left of b2.x
			Assert::IsTrue(ret1.a.pos_cs.y > ret1.c.pos_cs.y, (wss.str() + L"A1^C1").c_str());	 // a1.y should still be above of c1.y
			Assert::IsTrue(ret1.b.pos_cs.y < ret1.c.pos_cs.y, (wss.str() + L"B1vC1").c_str());	 // b1.y should still be below of c1.y
			Assert::IsTrue(ret2.a.pos_cs.y > ret2.c.pos_cs.y, (wss.str() + L"A2^C2").c_str());	 // a2.y should still be above of c2.y
			Assert::IsTrue(ret2.b.pos_cs.y < ret2.c.pos_cs.y, (wss.str() + L"B2vC2").c_str());	 // b2.y should still be below of c2.y

			return;
		}

		// Tests that if the B and C vertices are inside, the vertex order will stay intact
		TEST_METHOD(Vertex_Order_Stays_Intact_2_In_BC)
		{
			InterRenderTriangle ird;
			ird.a.pos_cs = { -900, 0, 4, 612 };
			ird.b.pos_cs = { 0, 100, 4, 547 };
			ird.c.pos_cs = { 0, -100, 4, 547 };

			std::vector<InterRenderTriangle> res = ClippingEngine::Clip(ird);
			Assert::AreEqual(2, (int)res.size(), L"Triangle return count mismatch");
			InterRenderTriangle ret1 = res[0];
			InterRenderTriangle ret2 = res[1];

			std::wstringstream wss;
			wss << std::endl
				<< "a1:" << ret1.a.pos_cs << std::endl
				<< "b1:" << ret1.b.pos_cs << std::endl
				<< "c1:" << ret1.c.pos_cs << std::endl
				<< std::endl
				<< "a2:" << ret2.a.pos_cs << std::endl
				<< "b2:" << ret2.b.pos_cs << std::endl
				<< "c2:" << ret2.c.pos_cs << std::endl
				<< std::endl;

			Assert::IsTrue(ret1.b.pos_cs == ird.b.pos_cs, (wss.str() + L"B1").c_str());    // b1 should stay untouched
			Assert::IsTrue(ret1.c.pos_cs == ird.c.pos_cs, (wss.str() + L"C1").c_str());    // c1 should stay untouched
			Assert::IsTrue(ret2.c.pos_cs == ird.c.pos_cs, (wss.str() + L"C2").c_str());    // c2 should stay untouched
			Assert::IsTrue(ret1.a.pos_cs.x < ret1.b.pos_cs.x, (wss.str() + L"A1<B1").c_str());	 // a1.x should still be to the left of b1.x
			Assert::IsTrue(ret2.a.pos_cs.x < ret2.c.pos_cs.x, (wss.str() + L"A2<C2").c_str());	 // a2.x should still be to the left of c2.x
			Assert::IsTrue(ret1.b.pos_cs.y > ret1.a.pos_cs.y, (wss.str() + L"B1^A1").c_str());	 // b1.y should still be above of a1.y
			Assert::IsTrue(ret1.c.pos_cs.y < ret1.a.pos_cs.y, (wss.str() + L"C1vA1").c_str());	 // c1.y should still be below of a1.y
			Assert::IsTrue(ret2.b.pos_cs.y > ret2.a.pos_cs.y, (wss.str() + L"B2^A2").c_str());	 // b2.y should still be above of a2.y
			Assert::IsTrue(ret2.c.pos_cs.y < ret2.a.pos_cs.y, (wss.str() + L"C2vA2").c_str());	 // c2.y should still be below of a2.y

			return;
		}

		// Tests that if the C and A vertices are inside, the vertex order will stay intact
		TEST_METHOD(Vertex_Order_Stays_Intact_2_In_CA)
		{
			InterRenderTriangle ird;
			ird.a.pos_cs = { 0, 100, 4, 547 };
			ird.b.pos_cs = { -900, 0, 4, 612 };
			ird.c.pos_cs = { 0, -100, 4, 547 };

			std::vector<InterRenderTriangle> res = ClippingEngine::Clip(ird);
			Assert::AreEqual(2, (int)res.size(), L"Triangle return count mismatch");
			InterRenderTriangle ret1 = res[0];
			InterRenderTriangle ret2 = res[1];

			std::wstringstream wss;
			wss << std::endl
				<< "a1:" << ret1.a.pos_cs << std::endl
				<< "b1:" << ret1.b.pos_cs << std::endl
				<< "c1:" << ret1.c.pos_cs << std::endl
				<< std::endl
				<< "a2:" << ret2.a.pos_cs << std::endl
				<< "b2:" << ret2.b.pos_cs << std::endl
				<< "c2:" << ret2.c.pos_cs << std::endl
				<< std::endl;

			Assert::IsTrue(ret1.a.pos_cs == ird.a.pos_cs, (wss.str() + L"A1").c_str());    // a1 should stay untouched
			Assert::IsTrue(ret1.c.pos_cs == ird.c.pos_cs, (wss.str() + L"C1").c_str());    // c1 should stay untouched
			Assert::IsTrue(ret2.c.pos_cs == ird.c.pos_cs, (wss.str() + L"C2").c_str());    // c2 should stay untouched
			Assert::IsTrue(ret1.b.pos_cs.x < ret1.a.pos_cs.x, (wss.str() + L"B1<A1").c_str());	 // b1.x should still be to the left of a1.x
			Assert::IsTrue(ret2.b.pos_cs.x < ret2.c.pos_cs.x, (wss.str() + L"B2<C2").c_str());	 // b2.x should still be to the left of c2.x
			Assert::IsTrue(ret1.a.pos_cs.y > ret1.b.pos_cs.y, (wss.str() + L"A1^B1").c_str());	 // a1.y should still be above of b1.y
			Assert::IsTrue(ret1.c.pos_cs.y < ret1.b.pos_cs.y, (wss.str() + L"C1vB1").c_str());	 // c1.y should still be below of b1.y
			Assert::IsTrue(ret2.a.pos_cs.y > ret2.b.pos_cs.y, (wss.str() + L"A2^B2").c_str());	 // a2.y should still be above of b2.y
			Assert::IsTrue(ret2.c.pos_cs.y < ret2.b.pos_cs.y, (wss.str() + L"C2vB2").c_str());	 // c2.y should still be below of b2.y

			return;
		}

	// No memory leak testing necessary, because it does not work with pointers
	};
}
