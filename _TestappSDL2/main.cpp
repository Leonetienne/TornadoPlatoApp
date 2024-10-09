#include <SDL2/SDL.h>
#include <iostream>
#include "../Plato/Vector.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "../Plato/Clock.h"
#include "TestFixture.h"
#include "Test__MC.h"
#include "CameraKeyboardControl.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const Vector2i resolution(800, 600);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow(
        "FPS Camera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        resolution.x, resolution.y, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    bool running = true;

    // Lock mouse to the window and make it invisible
    SDL_SetRelativeMouseMode(SDL_TRUE); // Lock mouse and capture mouse delta

    std::cout << "Initializing..." << std::endl;
    Input::EventManager::Init();
    // Create a renderer and a test object

    Renderer renderer(resolution);

    std::cout << "Creating camera..." << std::endl;
	Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
	Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolution, 90, 0.001, 10);
	cameraYPivot->worldObject->SetId("main_camera_ypiv");
	camera->SetAsMainCamera();
	// Let's add a CameraKeyboardControl component to the camera by default
	camera->worldObject->AddComponent<CameraKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);

    // Instantiate the test scene
    Test__MC testScene;

    Clock frametimer;
    double frametime = 1000.0/60.0;
    SDL_Event event;
    int mouseDeltaX = 0, mouseDeltaY = 0;
    while (running) {
        mouseDeltaX = 0;
        mouseDeltaY = 0;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                // Capture the relative mouse movement
                mouseDeltaX = event.motion.xrel;
                mouseDeltaY = event.motion.yrel;
            }
        }

        WorldObjectManager::DeleteFlaggedObjects();
        testScene.Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);

        renderer.BeginFrame();
        WorldObjectManager::CallHook__Render(&renderer);
        renderer.Render();

        Input::EventManager::Digest();

        frametime = frametimer.GetElapsedTime().AsMilliseconds();
        frametimer.Reset();
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();

    //WorldObjectManager::Free();
    //ResourceManager::Free();

    return 0;
}

