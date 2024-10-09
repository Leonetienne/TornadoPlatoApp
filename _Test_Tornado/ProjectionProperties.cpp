#include "../_TestingUtilities/Catch2.h"
#include "../Tornado/ProjectionProperties.h"
#include "../Eule/Math.h"
#include <random>

using namespace TorGL;
using Eule::Math;

TEST_CASE("_ProjectionProperties Tests", "[projection]") {
    std::mt19937 rng((std::random_device())());

    SECTION("Constructor_Values_Match") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        REQUIRE(props.GetResolution().x == 1920);
        REQUIRE(props.GetResolution().y == 1080);
        REQUIRE(props.GetAspectRatio() == Approx(1920.0 / 1080.0));
        REQUIRE(props.GetFov() == Approx(90.0));
        REQUIRE(props.GetFarclip() == Approx(100.0));
        REQUIRE(props.GetSqrFarclip() == 10000.0l);
        REQUIRE(props.GetHalfResolution() == Vector2d(960, 540));
        REQUIRE(props.GetNearclip() == Approx(2.0));
    }

    SECTION("Can_Set_Resolution") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        props.SetResolution({ 3840, 1440 });

        REQUIRE(props.GetResolution().x == 3840);
        REQUIRE(props.GetResolution().y == 1440);
        REQUIRE(props.GetAspectRatio() == Approx(3840.0 / 1440.0));
        REQUIRE(props.GetHalfResolution() == Vector2d(1920, 720));
    }

    SECTION("Can_Set_Fov") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        props.SetFov(65);

        REQUIRE(props.GetFov() == Approx(65.0));
    }

    SECTION("Can_Set_Nearclip") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        props.SetNearclip(6);

        REQUIRE(props.GetNearclip() == Approx(6.0));
    }

    SECTION("Can_Set_Farclip") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        props.SetFarclip(200);

        REQUIRE(props.GetFarclip() == Approx(200.0));
        REQUIRE(props.GetSqrFarclip() == 40000.0l);
    }

    SECTION("Is_ProjectionMatrix_Correct") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        Matrix4x4 comparison;
        comparison[0] = { 0.5625, 0,            0,            0 };
        comparison[1] = {      0, 1,            0,            0 };
        comparison[2] = {      0, 0, -1.040816327, -4.081632653 };
        comparison[3] = {      0, 0,           -1,            1 };

        for (std::size_t y = 0; y < 4; y++) {
            for (std::size_t x = 0; x < 4; x++) {
                REQUIRE(Math::Similar(comparison[y][x], props.GetProjectionMatrix()[y][x]));
            }
        }
    }

    SECTION("Setting_Anything_Updates_Matrix") {
        ProjectionProperties props(
            Vector2i(1920, 1080),
            90,
            2,
            100
        );

        Matrix4x4 origMat;

        // Change resolution
        origMat = props.GetProjectionMatrix();
        props.SetResolution({ 800, 600 });
        REQUIRE_FALSE(origMat == props.GetProjectionMatrix());

        // Change fov
        origMat = props.GetProjectionMatrix();
        props.SetFov(60);
        REQUIRE_FALSE(origMat == props.GetProjectionMatrix());

        // Change nearclip
        origMat = props.GetProjectionMatrix();
        props.SetNearclip(99);
        REQUIRE_FALSE(origMat == props.GetProjectionMatrix());

        // Change farclip
        origMat = props.GetProjectionMatrix();
        props.SetFarclip(8888);
        REQUIRE_FALSE(origMat == props.GetProjectionMatrix());
    }
}

