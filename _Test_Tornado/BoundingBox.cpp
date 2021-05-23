#include "CppUnitTest.h"
#include "../Tornado/BoundingBox.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Misc
{
	TEST_CLASS(_BoundingBox)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_BoundingBox()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that all vertices can be set individually, and at once
		TEST_METHOD(Can_Set_Each_Vertex)
		{
			// All vertex values are unique
			BoundingBox bb;
			bb.SetVertex(BoundingBox::FRONT | BoundingBox::LEFT  | BoundingBox::BOTTOM, Vector3d(-1, -1,  1) * 1);
			bb.SetVertex(BoundingBox::FRONT | BoundingBox::LEFT  | BoundingBox::TOP,	Vector3d(-1,  1,  1) * 2);
			bb.SetVertex(BoundingBox::BACK	| BoundingBox::LEFT  | BoundingBox::BOTTOM, Vector3d(-1, -1, -1) * 3);
			bb.SetVertex(BoundingBox::BACK	| BoundingBox::LEFT  | BoundingBox::TOP,	Vector3d(-1,  1, -1) * 4);
			bb.SetVertex(BoundingBox::FRONT | BoundingBox::RIGHT | BoundingBox::BOTTOM, Vector3d( 1, -1,  1) * 5);
			bb.SetVertex(BoundingBox::FRONT | BoundingBox::RIGHT | BoundingBox::TOP,	Vector3d( 1,  1,  1) * 6);
			bb.SetVertex(BoundingBox::BACK	| BoundingBox::RIGHT | BoundingBox::BOTTOM, Vector3d( 1, -1, -1) * 7);
			bb.SetVertex(BoundingBox::BACK	| BoundingBox::RIGHT | BoundingBox::TOP,	Vector3d( 1,  1, -1) * 8);

			Assert::IsTrue(bb.GetVertex(BoundingBox::FRONT	| BoundingBox::LEFT  | BoundingBox::BOTTOM) == (Vector3d(-1, -1,  1) * 1), L"FRONT|LEFT|BOTTOM");
			Assert::IsTrue(bb.GetVertex(BoundingBox::FRONT	| BoundingBox::LEFT  | BoundingBox::TOP)	== (Vector3d(-1,  1,  1) * 2), L"FRONT|LEFT|TOP");
			Assert::IsTrue(bb.GetVertex(BoundingBox::BACK	| BoundingBox::LEFT  | BoundingBox::BOTTOM) == (Vector3d(-1, -1, -1) * 3), L"BACK|LEFT|BOTTOM");
			Assert::IsTrue(bb.GetVertex(BoundingBox::BACK	| BoundingBox::LEFT  | BoundingBox::TOP)	== (Vector3d(-1,  1, -1) * 4), L"BACK|LEFT|TOP");
			Assert::IsTrue(bb.GetVertex(BoundingBox::FRONT	| BoundingBox::RIGHT | BoundingBox::BOTTOM) == (Vector3d( 1, -1,  1) * 5), L"FRONT|RIGHT|BOTTOM");
			Assert::IsTrue(bb.GetVertex(BoundingBox::FRONT	| BoundingBox::RIGHT | BoundingBox::TOP)	== (Vector3d( 1,  1,  1) * 6), L"FRONT|RIGHT|TOP");
			Assert::IsTrue(bb.GetVertex(BoundingBox::BACK	| BoundingBox::RIGHT | BoundingBox::BOTTOM) == (Vector3d( 1, -1, -1) * 7), L"BACK|RIGHT|BOTTOM");
			Assert::IsTrue(bb.GetVertex(BoundingBox::BACK	| BoundingBox::RIGHT | BoundingBox::TOP)	== (Vector3d( 1,  1, -1) * 8), L"BACK|RIGHT|TOP");

			return;
		}
	};
}
