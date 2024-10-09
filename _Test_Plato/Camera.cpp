#include "../_TestingUtilities/Catch2.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/Camera.h"
#include <random>

using namespace Plato;
using namespace Plato::Components;

#define DUMMY_CAM_ARGUMENTS Vector2i(10,10), 90, 0.1, 100

#define TEST_START WorldObjectManager::Free();
#define TEST_END WorldObjectManager::Free();

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests that Camera::GetMainCamera() returns the correct (main) camera.
TEST_CASE(__FILE__"/GetMainCamera", "[Camera]")
{
    TEST_START

    // Setup
    Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

    cam1->SetAsMainCamera();

    // Exercise
    Camera* mainCamera = Camera::GetMainCamera();

    // Verify
    REQUIRE(cam1 == mainCamera);

    TEST_END
    return;
}

// Tests that a main camera can be set
TEST_CASE(__FILE__"/Can_Set_Main_Camera", "[Camera]")
{
    TEST_START

    // Setup
    Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

    // Exercise
    cam1->SetAsMainCamera();
    // Verify
    REQUIRE(cam1 == Camera::GetMainCamera());

    // Exercise
    cam3->SetAsMainCamera();
    // Verify
    REQUIRE(cam3 == Camera::GetMainCamera());

    TEST_END
    return;
}

// Tests that the main camera does always have the id "main_camera"
TEST_CASE(__FILE__"/Main_Camera_Always_Has_Id_main_camera", "[Camera]")
{
    TEST_START

    // Setup
    Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

    // Exercise
    cam1->SetAsMainCamera();
    // Verify
    REQUIRE(cam1->worldObject == WorldObjectManager::FindObjectById("main_camera"));

    // Exercise
    cam2->SetAsMainCamera();
    // Verify
    REQUIRE(cam2->worldObject == WorldObjectManager::FindObjectById("main_camera"));

    // Exercise
    cam3->SetAsMainCamera();
    // Verify
    REQUIRE(cam3->worldObject == WorldObjectManager::FindObjectById("main_camera"));

    TEST_END
    return;
}

// Tests that the main camera gets set to nullptr, when it gets deleted via WorldObjectManager::Free()
TEST_CASE(__FILE__"/Deleted_Main_Camera_Is_Nullptr_Free", "[Camera]")
{
    TEST_START;

    // Setup
    Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    
    REQUIRE(cam0 == Camera::GetMainCamera());

    // Exercise
    WorldObjectManager::Free();

    // Verify
    REQUIRE(Camera::GetMainCamera() == nullptr);

    TEST_END;
    return;
}

// Tests that the main camera gets set to nullptr, when it gets deleted via Destroy
TEST_CASE(__FILE__"/Deleted_Main_Camera_Is_Nullptr_Destroy", "[Camera]")
{
    TEST_START;

    // Setup
    Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

    REQUIRE(cam0 == Camera::GetMainCamera());

    // Exercise
    cam0->worldObject->Destroy();
    WorldObjectManager::DeleteFlaggedObjects();

    // Verify
    REQUIRE(Camera::GetMainCamera() == nullptr);

    TEST_END;
    return;
}

// Tests that the first camera created is automatically the main camera
TEST_CASE(__FILE__"/First_Camera_Is_Main_Camera", "[Camera]")
{
    TEST_START

    // Setup, Exercise
    Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
    Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

    // Verify
    REQUIRE(cam0 == Camera::GetMainCamera());

    TEST_END
    return;
}

#undef DUMMY_CAM_ARGUMENTS #undef TEST_START
#undef TEST_START
#undef TEST_END

