#include "HeadlessFPS.h"

HeadlessFPS::HeadlessFPS(const Vector2i& resolution)
    :
    renderer(resolution)
{

    // Set up main camera transform
    Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation

    Components::Camera* camera =
            WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolution, 90, 0.001, 10);

    cameraYPivot->worldObject->SetId("main_camera_ypiv");
    camera->SetAsMainCamera();

    // Let's add a CameraKeyboardControl component to the camera by default
    camera->worldObject->AddComponent<CameraKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);

    // Create test fixture (fps controller)
    testFixture = new TEST_TO_RUN();

    return;
}

const TorGL::PixelBuffer<3> *HeadlessFPS::GetRenderPixelBuffer() const {
    return renderer.GetPixelBuffer();
}

HeadlessFPS::~HeadlessFPS() {
    WorldObjectManager::Free();
    ResourceManager::Free();

    delete testFixture;
    testFixture = nullptr;

    return;
}

void HeadlessFPS::Loop() {
    // Get elapsed time for last frame
    const double elapsedTime = frametimeClock.GetElapsedTime().AsMilliseconds();
    curFps = 1000.0 / elapsedTime;

    // Reset frametime clock
    frametimeClock.Reset();

    WorldObjectManager::DeleteFlaggedObjects();

    // Update test fixture
    testFixture->Update(elapsedTime);
    WorldObjectManager::CallHook__Update(elapsedTime);
    WorldObjectManager::CallHook__LateUpdate(elapsedTime);

    // Clear frame
    renderer.BeginFrame();

    // Render test fixture
    testFixture->Render(&renderer);
    WorldObjectManager::CallHook__Render(&renderer);

    // Render frame
    renderer.Render();

    // Digest events
    Input::EventManager::Digest();
}
