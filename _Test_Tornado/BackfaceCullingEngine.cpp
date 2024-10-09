#include "../_TestingUtilities/Catch2.h"
#include "../Tornado/BackfaceCullingEngine.h"
#include <random>
#include <sstream>

using namespace TorGL;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests that a triangle not facing away is visible
TEST_CASE(__FILE__"/Does_Not_Clip_Completely_Visibe_Triangle", "[BackfaceCullingEngine]")
{
    // Setup
    WorkerPool wp(0);
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
    REQUIRE(std::size_t(1) == ret.size());

    return;
}

// Tests that a triangle not facing away is visible
TEST_CASE(__FILE__"/Does_Clip_Completely_Backfacing_Triangle", "[BackfaceCullingEngine]")
{
    // Setup
    WorkerPool wp(0);
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
    REQUIRE(std::size_t(0) == ret.size());

    return;
}

