#include "CppUnitTest.h"
#include "../Tornado/RenderTriangle3D.h"
#include "../Tornado/BackfaceCullingEngine.h"
#include "../Tornado/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Engines
{
	TEST_CLASS(_BackfaceCullingEngine)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_BackfaceCullingEngine()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a triangle not facing away is visible
		TEST_METHOD(Does_Not_Clip_Completely_Visibe_Triangle)
		{
			// Setup
			BackfaceCullingEngine cullingEngine(&WorkerPool(24));

			RenderTriangle3D rd;
			rd.a.pos_worldSpace = Vector3d(1.4663, 5.5052, 0.99218);
			rd.b.pos_worldSpace = Vector3d(1.76219, 7.48314, 1.00722);
			rd.c.pos_worldSpace = Vector3d(1.51444, 5.51321, -1.00722);

			// Exercise
			std::vector<RenderTriangle3D> tris;
			tris.push_back(rd);

			cullingEngine.BeginBatch();
			cullingEngine.RegisterRenderTriangle(&tris[0]);
			cullingEngine.Cull();
			std::vector<const RenderTriangle3D*> ret = cullingEngine.Finish();

			// Verify
			Assert::AreEqual(std::size_t(1), ret.size());

			return;
		}

		// Tests that a triangle not facing away is visible
		TEST_METHOD(Does_Clip_Completely_Backfacing_Triangle)
		{
			// Setup
			BackfaceCullingEngine cullingEngine(&WorkerPool(24));

			RenderTriangle3D rd;
			rd.a.pos_worldSpace = Vector3d(100 + 1.4663, 5.5052, 0.99218);
			rd.b.pos_worldSpace = Vector3d(100 + 1.76219, 7.48314, 1.00722);
			rd.c.pos_worldSpace = Vector3d(100 + 1.51444, 5.51321, -1.00722);

			// Exercise
			std::vector<RenderTriangle3D> tris;
			tris.push_back(rd);

			cullingEngine.BeginBatch();
			cullingEngine.RegisterRenderTriangle(&tris[0]);
			cullingEngine.Cull();
			std::vector<const RenderTriangle3D*> ret = cullingEngine.Finish();

			// Verify
			Assert::AreEqual(std::size_t(0), ret.size());

			return;
		}

	};
}
