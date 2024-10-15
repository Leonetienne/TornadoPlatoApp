#pragma once
#include "RenderWindow.h"
#include "../Plato/Vector.h"
#include "../Tornado/PixelBuffer.h"

using namespace Plato;

class TerminalBlockCharacterRenderWindow : public RenderWindow
{
public:
    TerminalBlockCharacterRenderWindow(const Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer);

    void Close() override;

    void PollEvents() override;
    void RedrawWindow() override;

    // Not implemented
    void SetWindowTitle(const std::string& title) override;


private:
    // Gets whether a key just pressed
    bool WasKeyPressed();
    // Gets the current key pressed, -1 if none
    char GetKeyPressed();

    char keyLastFrame = -1;
};

