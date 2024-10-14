#define _BENCHMARK_CONTEXT

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

struct PerformanceMetric {
    int totalFrameTime;
    int uptateHooks;
    int render;
    int platorender_beginFrame;
    int platorender_resolveCameraSpaceVertices;
    int platorender_registerTornadoObjects;
    int tornadorender_beginFrame;
    int tornadorender_render;
    int sdlDraw;
};

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

    std::vector<PerformanceMetric> performanceMetrics;
    performanceMetrics.reserve(3000);

    // Set up the main loop
    Clock frametimer;
    double statsCaptureIntervalMs = 100.0;
    double frametime = 1000.0 / 10.0;

    // Set up performance timers
    Clock statsCaptureTimer;
    Clock perfTimer;


    while (renderWindow.IsRunning()) {
        // Poll the SDL2 windows events
        renderWindow.PollEvents();

        // Digest plato events
        Input::EventManager::Digest();

        // Delete objects that were flagged to be deleted
        WorldObjectManager::DeleteFlaggedObjects();

        // Tick update hooks
        perfTimer.Reset();
        BenchmarkScene.Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);
        const double updateHooksTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Render the frame
        perfTimer.Reset();
        renderer.BeginFrame();
        WorldObjectManager::CallHook__Render(&renderer);
        renderer.Render();
        const double platoRenderTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Display the frame
        perfTimer.Reset();
        renderWindow.RedrawWindow();
        const double sdlDrawTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Capture how long the frame took to complete
        frametime = frametimer.GetElapsedTime().AsMilliseconds();

        // Capture performance metrics every set time intervals
        if (statsCaptureTimer.GetElapsedTime().AsMilliseconds() >= statsCaptureIntervalMs) {
            statsCaptureTimer.Reset();
            performanceMetrics.push_back({
                .totalFrameTime = (int)frametime,
                .uptateHooks = (int)updateHooksTime,
                .render = (int)platoRenderTime,
                .platorender_beginFrame = (int)renderer._benchmark_GetBeginFrameTime(),
                .platorender_resolveCameraSpaceVertices = (int)renderer._benchmark_GetResolveCameraSpaceVerticesTime(),
                .platorender_registerTornadoObjects = (int)renderer._benchmark_RegisterTornadoObjectsTime(),
                .tornadorender_beginFrame = (int)renderer._benchmark_GetTornadoBeginFrameTime(),
                .tornadorender_render = (int)renderer._benchmark_GetTornadoRenderTime(),
                .sdlDraw = (int)sdlDrawTime,
            });
        }

        // Only reset the frame timer now to exclude the time it takes
        // to record the new performance metrics struct from the frametimer value
        frametimer.Reset();
    }

    // Clean up
    WorldObjectManager::Free();
    ResourceManager::Free();

    // Write performance metrics
    const std::string metricsDir = "./dataplotter/performance-metrics";
    std::filesystem::create_directories(metricsDir);
    std::ofstream csvFs(metricsDir + "/last-run.csv", std::ofstream::out);
    if (csvFs.good()) {
        std::stringstream metricsCsvSs;
        std::cout << "Writing performance metrics to " << metricsDir << "/last-run-fps.csv..." << std::endl;
        csvFs <<
            "frametime,"
            "updateHooks,"
            "platoRender,"
            "platoRender_beginFrame,"
            "platoRender_resolveCameraSpaceVertices,"
            "platoRender_registerTornadoObjects,"
            "platoRender_tornadoRender_beginFrame,"
            "platoRender_tornadoRender_render"
        << std::endl;
        for (const PerformanceMetric& it : performanceMetrics) {
            csvFs
                << it.totalFrameTime << ','
                << it.uptateHooks << ','
                << it.render << ','
                << it.platorender_beginFrame << ','
                << it.platorender_resolveCameraSpaceVertices << ','
                << it.platorender_registerTornadoObjects << ','
                << it.tornadorender_beginFrame << ','
                << it.tornadorender_render
                << std::endl;
        }
        csvFs.flush();
        csvFs.close();
    }
    else {
        std::cerr << "Failed to open filestream for write of " << metricsDir << " /last-run-fps.csv! Ignoring..." << std::endl;
    }

    return 0;
}

