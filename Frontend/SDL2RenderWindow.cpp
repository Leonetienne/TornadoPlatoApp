#include "SDL2RenderWindow.h"
#include "RenderWindow.h"
#include "../Plato/EventManager.h"
#include <cstdio>
#include <iostream>

using namespace Plato;

SDL2RenderWindow::SDL2RenderWindow(const Eule::Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer) :
RenderWindow(resolution, name, renderResultPixelBuffer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(-1);

    }

    // Create an SDL window
    sdlWindow = SDL_CreateWindow(
        name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
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

SDL2RenderWindow::~SDL2RenderWindow()
{
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
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

        // Handle mouse motion
        else if (sdlEvent.type == SDL_MOUSEMOTION) {
            // Capture the relative mouse movement
            mouseDelta.x = sdlEvent.motion.xrel;
            mouseDelta.y = sdlEvent.motion.yrel;
            mousePos.x = sdlEvent.motion.x;
            mousePos.y = sdlEvent.motion.y;

            Input::EventManager::RegisterEventMouseDelta(mouseDelta);
            Input::EventManager::RegisterEventMousePosition(mousePos, mousePos);
        }

        // Handle mouse wheel scroll
        else if (sdlEvent.type == SDL_MOUSEWHEEL) {
            double mouseWheelDelta = sdlEvent.wheel.y; // y is vertical scrolling, x is horizontal scrolling
            Input::EventManager::RegisterEventMousewheelDelta(mouseWheelDelta);
        }

        // Handle mouse click down
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
            Input::EventManager::RegisterEventKeyDown(
                SDLMouseButton2PlatoKey(sdlEvent.button.button)
            );
        }

        // Handle mouse click up
        else if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
            Input::EventManager::RegisterEventKeyUp(
                SDLMouseButton2PlatoKey(sdlEvent.button.button)
            );
        }

        // Handle key down
        else if (sdlEvent.type == SDL_KEYDOWN) {
            SDL_KeyboardEvent* keyEvent = &sdlEvent.key;
            Input::EventManager::RegisterEventKeyDown(
                SDLKeyCode2PlatoKey((SDL_KeyCode)keyEvent->keysym.sym)
            );
        }

        // Handle key up
        else if (sdlEvent.type == SDL_KEYUP) {
            SDL_KeyboardEvent* keyEvent = &sdlEvent.key;
            Input::EventManager::RegisterEventKeyUp(
                SDLKeyCode2PlatoKey((SDL_KeyCode)keyEvent->keysym.sym)
            );
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
        // starts with one so we can use memcpy correctly (range now from [0; resolution.y - 1])
        for (std::size_t y = 1; y <= resolution.y; y++) {
            std::memcpy(
              //y-1 so we can start from 0 for the dest
              static_cast<uint8_t*>(pixels) + (y-1) * pitch, //dest
                renderResultPixelBuffer->GetRawData() + (resolution.y - y) * pitch, //src
                pitch // count
                       );
        }
        SDL_UnlockTexture(sdlTexture);
    }
    else {
        std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
    }
    // Clear the renderer
    SDL_RenderClear(sdlRenderer);
    // Copy the texture to t  he renderer
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
}

void SDL2RenderWindow::EnableMouseCameraControlMode()
{
    // Lock mouse to the window and make it invisible
    SDL_SetRelativeMouseMode(SDL_TRUE); // Lock mouse and capture mouse delta
}

Input::KEY_CODE SDL2RenderWindow::SDLKeyCode2PlatoKey(SDL_KeyCode sdlkey)
{
    // https://github.com/libsdl-org/SDL/blob/SDL2/include/SDL_keycode.h
    switch (sdlkey) {
        // LETTERS
        case SDLK_a:
            return Input::KEY_CODE::A;
        case SDLK_b:
            return Input::KEY_CODE::B;
        case SDLK_c:
            return Input::KEY_CODE::C;
        case SDLK_d:
            return Input::KEY_CODE::D;
        case SDLK_e:
            return Input::KEY_CODE::E;
        case SDLK_f:
            return Input::KEY_CODE::F;
        case SDLK_g:
            return Input::KEY_CODE::G;
        case SDLK_h:
            return Input::KEY_CODE::H;
        case SDLK_i:
            return Input::KEY_CODE::I;
        case SDLK_j:
            return Input::KEY_CODE::J;
        case SDLK_k:
            return Input::KEY_CODE::K;
        case SDLK_l:
            return Input::KEY_CODE::L;
        case SDLK_m:
            return Input::KEY_CODE::M;
        case SDLK_n:
            return Input::KEY_CODE::N;
        case SDLK_o:
            return Input::KEY_CODE::O;
        case SDLK_p:
            return Input::KEY_CODE::P;
        case SDLK_q:
            return Input::KEY_CODE::Q;
        case SDLK_r:
            return Input::KEY_CODE::R;
        case SDLK_s:
            return Input::KEY_CODE::S;
        case SDLK_t:
            return Input::KEY_CODE::T;
        case SDLK_u:
            return Input::KEY_CODE::U;
        case SDLK_v:
            return Input::KEY_CODE::V;
        case SDLK_w:
            return Input::KEY_CODE::W;
        case SDLK_x:
            return Input::KEY_CODE::X;
        case SDLK_y:
            return Input::KEY_CODE::Y;
        case SDLK_z:
            return Input::KEY_CODE::Z;

        // NUMBER ROW
        case SDLK_1:
            return Input::KEY_CODE::NUM_1;
        case SDLK_2:
            return Input::KEY_CODE::NUM_2;
        case SDLK_3:
            return Input::KEY_CODE::NUM_3;
        case SDLK_4:
            return Input::KEY_CODE::NUM_4;
        case SDLK_5:
            return Input::KEY_CODE::NUM_5;
        case SDLK_6:
            return Input::KEY_CODE::NUM_6;
        case SDLK_7:
            return Input::KEY_CODE::NUM_7;
        case SDLK_8:
            return Input::KEY_CODE::NUM_8;
        case SDLK_9:
            return Input::KEY_CODE::NUM_9;
        case SDLK_0:
            return Input::KEY_CODE::NUM_0;

        // NUMPAD NUMBERS
        case SDLK_KP_1:
            return Input::KEY_CODE::NUMPAD_1;
        case SDLK_KP_2:
            return Input::KEY_CODE::NUMPAD_2;
        case SDLK_KP_3:
            return Input::KEY_CODE::NUMPAD_3;
        case SDLK_KP_4:
            return Input::KEY_CODE::NUMPAD_4;
        case SDLK_KP_5:
            return Input::KEY_CODE::NUMPAD_5;
        case SDLK_KP_6:
            return Input::KEY_CODE::NUMPAD_6;
        case SDLK_KP_7:
            return Input::KEY_CODE::NUMPAD_7;
        case SDLK_KP_8:
            return Input::KEY_CODE::NUMPAD_8;
        case SDLK_KP_9:
            return Input::KEY_CODE::NUMPAD_9;
        case SDLK_KP_0:
            return Input::KEY_CODE::NUMPAD_0;

        // NUMPAD CONTROLS
        case SDLK_NUMLOCKCLEAR:
            return Input::KEY_CODE::NUMPAD_NLOCK;
        case SDLK_KP_DIVIDE:
            return Input::KEY_CODE::DIVIDE;
        case SDLK_KP_MULTIPLY:
            return Input::KEY_CODE::MULTIPLY;
        case SDLK_KP_MINUS:
            return Input::KEY_CODE::SUBTRACT;
        case SDLK_KP_PLUS:
            return Input::KEY_CODE::ADD;

        // Symbols
        case SDLK_PERIOD:
            return Input::KEY_CODE::PERIOD;
        case SDLK_MINUS:
            return Input::KEY_CODE::MINUS;
        case SDLK_COMMA:
            return Input::KEY_CODE::COMMA;
        case SDLK_PLUS:
            return Input::KEY_CODE::PLUS;
        case SDLK_SPACE:
            return Input::KEY_CODE::SPACE;
        case SDLK_CARET:
            return Input::KEY_CODE::CIRCUMFLEX;

        // ARROW KEYS
        case SDLK_UP:
            return Input::KEY_CODE::ARROW_UP;
        case SDLK_DOWN:
            return Input::KEY_CODE::ARROW_DOWN;
        case SDLK_LEFT:
            return Input::KEY_CODE::ARROW_LEFT;
        case SDLK_RIGHT:
            return Input::KEY_CODE::ARROW_RIGHT;

        // FUNCTION KEYS
        case SDLK_F1:
            return Input::KEY_CODE::F1;
        case SDLK_F2:
            return Input::KEY_CODE::F2;
        case SDLK_F3:
            return Input::KEY_CODE::F3;
        case SDLK_F4:
            return Input::KEY_CODE::F4;
        case SDLK_F5:
            return Input::KEY_CODE::F5;
        case SDLK_F6:
            return Input::KEY_CODE::F6;
        case SDLK_F7:
            return Input::KEY_CODE::F7;
        case SDLK_F8:
            return Input::KEY_CODE::F8;
        case SDLK_F9:
            return Input::KEY_CODE::F9;
        case SDLK_F10:
            return Input::KEY_CODE::F10;
        case SDLK_F11:
            return Input::KEY_CODE::F11;
        case SDLK_F12:
            return Input::KEY_CODE::F12;
        case SDLK_F13:
            return Input::KEY_CODE::F13;
        case SDLK_F14:
            return Input::KEY_CODE::F14;
        case SDLK_F15:
            return Input::KEY_CODE::F15;
        case SDLK_F16:
            return Input::KEY_CODE::F16;
        case SDLK_F17:
            return Input::KEY_CODE::F17;
        case SDLK_F18:
            return Input::KEY_CODE::F18;
        case SDLK_F19:
            return Input::KEY_CODE::F19;
        case SDLK_F20:
            return Input::KEY_CODE::F20;
        case SDLK_F21:
            return Input::KEY_CODE::F21;
        case SDLK_F22:
            return Input::KEY_CODE::F22;
        case SDLK_F23:
            return Input::KEY_CODE::F23;
        case SDLK_F24:
            return Input::KEY_CODE::F24;

        // CONTROL KEYS
        case SDLK_ESCAPE:
            return Input::KEY_CODE::ESCAPE;
        case SDLK_BACKSPACE:
            return Input::KEY_CODE::BACK;
        case SDLK_TAB:
            return Input::KEY_CODE::TAB;
        case SDLK_LSHIFT:
            return Input::KEY_CODE::LSHIFT;
        case SDLK_RSHIFT:
            return Input::KEY_CODE::RSHIFT;
        case SDLK_LCTRL:
            return Input::KEY_CODE::LCONTROL;
        case SDLK_RCTRL:
            return Input::KEY_CODE::RCONTROL;
        case SDLK_LALT:
            return Input::KEY_CODE::LALT;
        case SDLK_RALT:
            return Input::KEY_CODE::RALT;
        case SDLK_CAPSLOCK:
            return Input::KEY_CODE::CAPSLOCK;
        case SDLK_INSERT:
            return Input::KEY_CODE::INSERT;
        case SDLK_DELETE:
            return Input::KEY_CODE::DEL;
        case SDLK_PRINTSCREEN:
            return Input::KEY_CODE::PRINT;
        case SDLK_PAUSE:
            return Input::KEY_CODE::PAUSE;
        case SDLK_SCROLLLOCK:
            return Input::KEY_CODE::SCROLL_LOCK;
        case SDLK_PAGEUP:
            return Input::KEY_CODE::PAGE_UP;
        case SDLK_PAGEDOWN:
            return Input::KEY_CODE::PAGE_DOWN;
        case SDLK_END:
            return Input::KEY_CODE::END;

        // If not defined, stay safe and do nothing.
        default:
            return Input::KEY_CODE::NONE;
    }
}

Input::KEY_CODE SDL2RenderWindow::SDLMouseButton2PlatoKey(Uint8 sdlkey)
{
    switch (sdlkey) {
        case SDL_BUTTON_LEFT:
            return Input::KEY_CODE::MOUSE_L;
        case SDL_BUTTON_RIGHT:
            return Input::KEY_CODE::MOUSE_R;
        case SDL_BUTTON_MIDDLE:
            return Input::KEY_CODE::MOUSE_M;
        case SDL_BUTTON_X1: // Back
            return Input::KEY_CODE::MOUSE_X1;
        case SDL_BUTTON_X2: // Next
            return Input::KEY_CODE::MOUSE_X2;

        // If not defined, stay safe and do nothing.
        default:
            return Input::KEY_CODE::NONE;
    }
}

