#pragma once
#include "../Plato/Vector.h"
#include "../Tornado/PixelBuffer.h"
#include <SDL2/SDL.h>

using namespace Plato;

class RenderWindow
{
public:
    RenderWindow(const Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer);

    void Close();

    const Eule::Vector2i& GetResolution() const;
    bool IsRunning() const;
    void PollEvents();
    void RedrawWindow();

    //! Will lock and hide the mouse cursor and enable relative mouse position mode
    void EnableMouseCameraControlMode();

private:
    const Vector2i resolution;
    const TorGL::PixelBuffer<3>* renderResultPixelBuffer;
    bool running = true;
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Event sdlEvent;
};

