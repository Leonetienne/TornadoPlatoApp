#include "CppUnitTest.h"
#include "../Tornado/BackfaceCullingEngine.h"
#include "Eule/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TorGL;

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
			WorkerPool wp(24);
			BackfaceCullingEngine cullingEngine(&wp);

			InterRenderTriangle ird;
			ird.a.pos_ndc = Vector3d(1, 0, 1);
			ird.b.pos_ndc = Vector3d(0, 1, 1);
			ird.c.pos_ndc = Vector3d(-1, 0, 1);

			// Exercise
			std::vector<InterRenderTriangle> tris;
			tris.push_back(ird);

			cullingEngine.BeginBatch();
			cullingEngine.RegisterInterRenderTriangle(&tris[0]);
			cullingEngine.Cull();
			std::vector<const InterRenderTriangle*> ret = cullingEngine.Finish();

			// Verify
			Assert::AreEqual(std::size_t(1), ret.size());

			return;
		}

		// Tests that a triangle not facing away is visible
		TEST_METHOD(Does_Clip_Completely_Backfacing_Triangle)
		{
			// Setup
			WorkerPool wp(24);
			BackfaceCullingEngine cullingEngine(&wp);

			InterRenderTriangle ird;
			ird.a.pos_ndc = Vector3d(-1, 0, 1);
			ird.b.pos_ndc = Vector3d(0, 1, 1);
			ird.c.pos_ndc = Vector3d(1, 0, 1);

			// Exercise
			std::vector<InterRenderTriangle> tris;
			tris.push_back(ird);

			cullingEngine.BeginBatch();
			cullingEngine.RegisterInterRenderTriangle(&tris[0]);
			cullingEngine.Cull();
			std::vector<const InterRenderTriangle*> ret = cullingEngine.Finish();

			// Verify
			Assert::AreEqual(std::size_t(0), ret.size());

			return;
		}

	};
}
