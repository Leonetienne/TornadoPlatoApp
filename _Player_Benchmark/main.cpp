#include <iostream>
#include "../Plato/Vector.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "../Plato/Clock.h"
#include "Benchmarkscene.h"
#include "RenderWindow.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    const Vector2i resolution(800*2, 600*1.5);

    // Set up a plato renderer
    Renderer renderer(resolution, 0, 0);

    // Set up an SDL2 window
    RenderWindow renderWindow(resolution, "TornadoPlato Benchmark", renderer.GetPixelBuffer());
    renderWindow.EnableMouseCameraControlMode();
    RenderWindow* renderWindowPointer = &renderWindow; // Dumb workaroud for lambda function

    // Init plato event manager
    Input::EventManager::Init();

    // Register logic for plato to close the render window
    Input::EventManager::RegisterReverseEventCallback(
        Input::REVERSE_EVENT_CALLBACK::EXIT, 
		[renderWindowPointer](const std::vector<double>& params)
		{
			renderWindowPointer->Close();
		}
    );

    // Instantiate benchmark scene
    Benchmarkscene BenchmarkScene;

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
        BenchmarkScene.Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);

        // Render the frame
        renderer.BeginFrame();
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

    return 0;
}

