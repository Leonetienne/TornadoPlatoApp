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
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <filesystem>

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

    // Set up a csv stream for performance metrics
    std::stringstream fpsCsvSs;

    // Set up the main loop
    Clock frametimer;
    Clock statsCaptureTimer;
    double statsCaptureIntervalMs = 100.0;
    double frametime = 1000.0 / 10.0;

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

        // Reset frame timer
        frametime = frametimer.GetElapsedTime().AsMilliseconds();
        frametimer.Reset();

        // Capture performance metrics every set time intervals
        if (statsCaptureTimer.GetElapsedTime().AsMilliseconds() >= statsCaptureIntervalMs) {
            statsCaptureTimer.Reset();
            fpsCsvSs << (int)(frametime + 0.5) << ",";
        }
    }

    // Clean up
    WorldObjectManager::Free();
    ResourceManager::Free();

    // Write performance metrics
    const std::string metricsDir = "./dataplotter/performance-metrics";
    std::filesystem::create_directories(metricsDir);
    std::ofstream fpsCsvFs(metricsDir + "/last-run-fps.csv", std::ofstream::out);
    if (fpsCsvFs.good()) {
        std::cout << "Writing fps stats to " << metricsDir << "/last-run-fps.csv..." << std::endl;
        fpsCsvFs << fpsCsvSs.str().substr(0,fpsCsvSs.str().length()-1); // Omit last comma separator
        fpsCsvFs.flush();
        fpsCsvFs.close();
    }
    else {
        std::cerr << "Failed to open filestream for write of " << metricsDir << " /last-run-fps.csv! Ignoring..." << std::endl;
    }

    return 0;
}

