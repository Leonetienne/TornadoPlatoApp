#include <cstdint>
#include <iostream>
#include "../Plato/Vector.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "../Plato/Clock.h"
#include "TestFixture.h"
#include "Test__MC.h"
#include "Test__Benchmarkscene.h"
#include "CameraFPSKeyboardControl.h"
#include "RenderWindow.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const Vector2i resolution(800*2, 600*1.5);

    // Set up a plato renderer
    Renderer renderer(resolution, 0, 0);

    // Set up an SDL2 window
    RenderWindow renderWindow(resolution, "SDL-Player", renderer.GetPixelBuffer());
    renderWindow.EnableMouseCameraControlMode();
    RenderWindow* renderWindowPointer = &renderWindow; // Dumb workaroud for lambda function

    // Init plato event manager
    Input::EventManager::Init();

    // Set up an FPS camera
    std::cout << "Creating camera..." << std::endl;
    Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
    Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolution, 90, 0.001, 10);
    cameraYPivot->worldObject->SetId("main_camera_ypiv");
    camera->SetAsMainCamera();
    // Let's add a CameraKeyboardControl component to the camera by default
    camera->worldObject->AddComponent<CameraFPSKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);

    // Register logic for plato to close the render window
    Input::EventManager::RegisterReverseEventCallback(
        Input::REVERSE_EVENT_CALLBACK::EXIT, 
		[renderWindowPointer](const std::vector<double>& params)
		{
			renderWindowPointer->Close();
		}
    );


    // Instantiate a scene
    Test__Benchmarkscene testScene;

    // Set up the main loop
    Clock frametimer;
    double frametime = 1000.0 / 60.0;

    while (renderWindow.IsRunning()) {
        // Poll the SDL2 windows events
        renderWindow.PollEvents();

        // Digest plato events
        Input::EventManager::Digest();

        // Delete objects that were flagged to be deleted
        WorldObjectManager::DeleteFlaggedObjects();

        // Tick update hooks
        testScene.Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);

        // Render the frame
        renderer.BeginFrame();
        WorldObjectManager::CallHook__Render(&renderer);
        renderer.Render();

        // Display the frame
        renderWindow.RedrawWindow();

        // Calculate how long the frame took to display
        frametime = frametimer.GetElapsedTime().AsMilliseconds();
        frametimer.Reset();
        std::cout << "FPS: " << 1000.0 / frametime << std::endl;
    }

    // Clean up
    WorldObjectManager::Free();
    ResourceManager::Free();

    return 0;
}

