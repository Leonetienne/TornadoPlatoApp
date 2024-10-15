#include "RenderWindow.h"

using namespace TorGL;
using namespace Plato;

RenderWindow::RenderWindow(const Vector2i& resolution, const std::string& name, const PixelBuffer<3>* renderResultPixelBuffer) :
resolution{resolution},
renderResultPixelBuffer{renderResultPixelBuffer}
{

}

bool RenderWindow::IsRunning() const
{
    return running;
}

const Vector2i& RenderWindow::GetResolution() const
{
    return resolution;
}

void RenderWindow::Close()
{
    running = false;
}

