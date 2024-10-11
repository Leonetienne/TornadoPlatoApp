#include <SDL2/SDL.h>
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
#include <algorithm>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const Vector2i resolution(800*2, 600*1.5);

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

    // Create an SDL renderer
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create a texture to hold pixel buffer
    SDL_Texture* texture = SDL_CreateTexture(
        sdlRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 
        resolution.x, resolution.y);
    
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;

    // Lock mouse to the window and make it invisible
    SDL_SetRelativeMouseMode(SDL_TRUE); // Lock mouse and capture mouse delta

    std::cout << "Initializing..." << std::endl;
    Input::EventManager::Init();
    // Create a renderer and a test object
    Renderer renderer(resolution, 0);

    std::cout << "Creating camera..." << std::endl;
    Transform* cameraYPivot = WorldObjectManager::NewWorldObject()->transform; // Necessary for camera rotation
    Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(resolution, 90, 0.001, 10);
    cameraYPivot->worldObject->SetId("main_camera_ypiv");
    camera->SetAsMainCamera();
    // Let's add a CameraKeyboardControl component to the camera by default
    camera->worldObject->AddComponent<CameraFPSKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);

    // Instantiate the test scene
    Test__Benchmarkscene testScene;

    Clock frametimer;
    double frametime = 1000.0 / 60.0;
    SDL_Event event;
    Vector2i mouseDelta;
    Vector2i mousePos;

    while (running) {
        mouseDelta = Vector2i(0,0);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                // Capture the relative mouse movement
                mouseDelta.x = event.motion.xrel;
                mouseDelta.y = event.motion.yrel;
                mousePos.x = event.motion.x;
                mousePos.y = event.motion.y;

                Input::EventManager::RegisterEventMouseDelta(mouseDelta);
                Input::EventManager::RegisterEventMousePosition(mousePos, mousePos);
            }
            else if (event.type == SDL_KEYDOWN) {
                SDL_KeyboardEvent* keyEvent = &event.key;
                Input::EventManager::RegisterEventKeyDown((Input::KEY_CODE)keyEvent->keysym.sym);
            }
            else if (event.type == SDL_KEYUP) {
                SDL_KeyboardEvent* keyEvent = &event.key;
                Input::EventManager::RegisterEventKeyUp((Input::KEY_CODE)keyEvent->keysym.sym);
            }
        }

        WorldObjectManager::DeleteFlaggedObjects();
        testScene.Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);

        renderer.BeginFrame();
        WorldObjectManager::CallHook__Render(&renderer);
        renderer.Render();

        // Update the texture with pixel data from the renderer
        void* pixels;
        int pitch;
        if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) == 0) {
            for (std::size_t y = 0; y < resolution.y; y++) {
                std::memcpy(static_cast<uint8_t*>(pixels) + y * pitch, renderer.GetPixelBuffer()->GetRawData() + (resolution.y - y) * pitch, pitch);
            }
            SDL_UnlockTexture(texture);
        } else {
            std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
        }

        // Clear the renderer
        SDL_RenderClear(sdlRenderer);

        // Copy the texture to the renderer
        SDL_RenderCopy(sdlRenderer, texture, nullptr, nullptr);

        // Present the renderer
        SDL_RenderPresent(sdlRenderer);

        Input::EventManager::Digest();

        frametime = frametimer.GetElapsedTime().AsMilliseconds();
        frametimer.Reset();

        //std::cout << "FPS: " << 1000.0 / frametime << std::endl;
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

