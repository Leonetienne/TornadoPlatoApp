#include "../Scenes/Fun/Dust2/Dust2Scene.h"
#include "../Prefabs/FPSCamera/FPSCameraPrefab.h"
#include "../Frontend/SDL2RenderWindow.h"
#include "../Frontend/RenderWindow.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "../Plato/Clock.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const Vector2i resolution(800*2, 600*1.5);

    // Set up a plato renderer
    Renderer renderer(resolution, 0, 0.2);

    // Set up an SDL2 window
    SDL2RenderWindow renderWindow(resolution, "SDL-Player", renderer.GetPixelBuffer());
    renderWindow.EnableMouseCameraControlMode();
    RenderWindow* pRenderWindow = (RenderWindow*)(&renderWindow); // Dumb workaroud for lambda function

    // Init plato event manager
    Input::EventManager::Init();

    // Set up an FPS camera
    (FPSCameraPrefab(FPSCameraPrefab::CONTROL_TYPE::LOOK_MOUSE)).Instantiate();


    // Register logic for plato to close the render window
    Input::EventManager::RegisterReverseEventCallback(
        Input::REVERSE_EVENT_CALLBACK::EXIT, 
		[pRenderWindow](const std::vector<double>& params)
		{
			pRenderWindow->Close();
		}
    );

    // Instantiate a scene
    Scene* scene = new Dust2Scene;

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
        scene->Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        scene->LateUpdate(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);
        // Render the frame
        renderer.BeginFrame();
        scene->Render(&renderer);
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
    delete scene;
    scene = nullptr;

    return 0;
}

