#include "SDL2RenderWindow.h"
#include "../Plato/EventManager.h"
#include "RenderWindow.h"
#include <iostream>

SDL2RenderWindow::SDL2RenderWindow(const Eule::Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer) :
RenderWindow(resolution, name, renderResultPixelBuffer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(-1);

    }

    // Create an SDL window
    sdlWindow = SDL_CreateWindow(
        "FPS Camera", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        resolution.x, resolution.y, SDL_WINDOW_SHOWN);

    if (!sdlWindow) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(-1);
    }

    // Create an SDL renderer
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        exit(-1);
    }

    // Create a texture to hold pixel buffer
    sdlTexture = SDL_CreateTexture(
        sdlRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 
        resolution.x, resolution.y);
    
    if (!sdlTexture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(sdlRenderer);
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        exit(-1);
    }
}

void SDL2RenderWindow::PollEvents()
{
    static Vector2i mouseDelta(0,0);
    static Vector2i mousePos(0,0);

    mouseDelta = Vector2i(0,0);

    while (SDL_PollEvent(&sdlEvent)) {
        if (sdlEvent.type == SDL_QUIT) {
            Close();
        }
        else if (sdlEvent.type == SDL_MOUSEMOTION) {
            // Capture the relative mouse movement
            mouseDelta.x = sdlEvent.motion.xrel;
            mouseDelta.y = sdlEvent.motion.yrel;
            mousePos.x = sdlEvent.motion.x;
            mousePos.y = sdlEvent.motion.y;

            Input::EventManager::RegisterEventMouseDelta(mouseDelta);
            Input::EventManager::RegisterEventMousePosition(mousePos, mousePos);
        }
        else if (sdlEvent.type == SDL_KEYDOWN) {
            SDL_KeyboardEvent* keyEvent = &sdlEvent.key;
            // TODO: remap more complex keys like shift
            Input::EventManager::RegisterEventKeyDown((Input::KEY_CODE)keyEvent->keysym.sym);
        }
        else if (sdlEvent.type == SDL_KEYUP) {
            SDL_KeyboardEvent* keyEvent = &sdlEvent.key;
            // TODO: remap more complex keys like shift
            Input::EventManager::RegisterEventKeyUp((Input::KEY_CODE)keyEvent->keysym.sym);
        }
    }
}

void SDL2RenderWindow::RedrawWindow()
{
    // Copy the tornado pixel buffer to the sdl2 texture
    void* pixels;
    int pitch;
    if (SDL_LockTexture(sdlTexture, nullptr, &pixels, &pitch) == 0) {
        // SDLs pixel row order is reversed...
        for (std::size_t y = 0; y < resolution.y; y++) {
            std::memcpy(
                static_cast<uint8_t*>(pixels) + y * pitch,
                renderResultPixelBuffer->GetRawData() + (resolution.y - y) * pitch,
                pitch
            );
        }
        SDL_UnlockTexture(sdlTexture);
    }
    else {
        std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
    }

    // Clear the renderer
    SDL_RenderClear(sdlRenderer);

    // Copy the texture to the renderer
    SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);

    // Present the renderer
    SDL_RenderPresent(sdlRenderer);
}
void SDL2RenderWindow::SetWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(sdlWindow, title.c_str());
}

void SDL2RenderWindow::Close()
{
    RenderWindow::Close();
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void SDL2RenderWindow::EnableMouseCameraControlMode()
{
    // Lock mouse to the window and make it invisible
    SDL_SetRelativeMouseMode(SDL_TRUE); // Lock mouse and capture mouse delta
}

