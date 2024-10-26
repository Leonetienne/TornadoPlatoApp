#pragma once
#include "RenderWindow.h"
#include "../Plato/KeyCode.h"
#include "../Plato/Vector.h"
#include "../Tornado/PixelBuffer.h"
#include <SDL2/SDL.h>

using namespace Plato;

class SDL2RenderWindow : public RenderWindow
{
public:
    SDL2RenderWindow(const Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer);
    ~SDL2RenderWindow();

    void Close() override;

    void PollEvents() override;
    void RedrawWindow() override;
    void SetWindowTitle(const std::string& title) override;

    //! Will lock and hide the mouse cursor and enable relative mouse position mode
    void EnableMouseCameraControlMode();

protected:
    static Plato::Input::KEY_CODE SDLKeyCode2PlatoKey(SDL_KeyCode sdlkey);
    static Plato::Input::KEY_CODE SDLMouseButton2PlatoKey(Uint8 sdlkey);

    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Event sdlEvent;

private:
};

