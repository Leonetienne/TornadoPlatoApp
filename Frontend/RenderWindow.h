#pragma once
#include "../Plato/Vector.h"
#include "../Tornado/PixelBuffer.h"

class RenderWindow
{
    public:
        RenderWindow(const Plato::Vector2i& resolution, const std::string& name, const TorGL::PixelBuffer<3>* renderResultPixelBuffer);
        virtual ~RenderWindow() = default;

        // Implement these!
        virtual void Close(); // Call me too!
        virtual void PollEvents() {};
        virtual void RedrawWindow() {};
        virtual void SetWindowTitle(const std::string& title) {};

        // These can stay as-is
        const Plato::Vector2i& GetResolution() const;
        bool IsRunning() const;

    protected:
        const Plato::Vector2i resolution;
        const TorGL::PixelBuffer<3>* renderResultPixelBuffer;
        bool running = true;
};

