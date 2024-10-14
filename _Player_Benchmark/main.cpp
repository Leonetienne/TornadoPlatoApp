#include "HighResModelScene.h"
#define _BENCHMARK_CONTEXT

#include <iostream>
#include "../Plato/Vector.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/EventManager.h"
#include "../Plato/Renderer.h"
#include "../Plato/Clock.h"
#include "../Plato/Application.h"
#include "RenderWindow.h"
#include "BenchmarkScene.h"
#include "CaveCamFlightScene.h"
#include "HighResModelScene.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <filesystem>

struct PerformanceMetric {
    double totalFrameTime;
    double uptateHooks;
    double render;
    double platorender_beginFrame;
    double platorender_resolveCameraSpaceVertices;
    double platorender_registerTornadoObjects;
    double tornadorender_beginFrame;
    double tornadorender_render;
    double tornadorender_render_perspectiveProjection;
    double tornadorender_render_cullBackfaces;
    double tornadorender_render_drawTriangles;
    double sdlDraw;
};

// Will first transform the mean value back to the average sum of individual values, then add a new value, and perform the mean-division again
double addValueToMean(double oldMean, double newVal, std::size_t old_n) {
    return ((oldMean * (double)old_n) + newVal) / (old_n + 1);
}

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

    // Instantiate benchmark scenes
    std::vector<BenchmarkScene*> benchmarkScenes ({
        new HighResModelScene(),
        new CaveCamFlightScene()
    });
    std::size_t currentBenchmarkSceneIndex = 0;
    benchmarkScenes[currentBenchmarkSceneIndex]->Start();
    renderWindow.SetWindowTitle(std::string("TornadoPlato Benchmark - ") + benchmarkScenes[currentBenchmarkSceneIndex]->GetName());

    // Register logic for plato to close the render window
    Input::EventManager::RegisterReverseEventCallback(
        Input::REVERSE_EVENT_CALLBACK::EXIT, 
		[renderWindowPointer](const std::vector<double>& params)
		{
			renderWindowPointer->Close();
		}
    );

    std::vector<PerformanceMetric> performanceMetrics;
    performanceMetrics.reserve(3000);

    // Set up the main loop
    Clock frametimer;
    double statsCaptureIntervalMs = 100.0;
    double frametime = 1000.0 / 10.0;

    // Set up performance timers
    Clock statsCaptureTimer;
    Clock perfTimer;
    std::size_t numMetricsSinceLastInterval = 0;
    PerformanceMetric currentMetric;


    while (renderWindow.IsRunning()) {
        // Poll the SDL2 windows events
        renderWindow.PollEvents();

        // Digest plato events
        //std::cout << "Digesting events" << std::endl;
        Input::EventManager::Digest();

        // Delete objects that were flagged to be deleted
        //std::cout << "Deleting flagged objects" << std::endl;
        WorldObjectManager::DeleteFlaggedObjects();

        // Tick update hooks
        //std::cout << "Ticking update hooks" << std::endl;
        perfTimer.Reset();
        benchmarkScenes[currentBenchmarkSceneIndex]->Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);
        const double updateHooksTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Render the frame
        //std::cout << "Rendering the frame" << std::endl;
        perfTimer.Reset();
        renderer.BeginFrame();
        WorldObjectManager::CallHook__Render(&renderer);
        renderer.Render();
        const double platoRenderTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Display the frame
        //std::cout << "Drawing the frame" << std::endl;
        perfTimer.Reset();
        renderWindow.RedrawWindow();
        const double sdlDrawTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Capture how long the frame took to complete
        frametime = frametimer.GetElapsedTime().AsMilliseconds();

        // Update current metrics values
        // If it already has values, update its means
        //std::cout << "Saving metrics" << std::endl;
        if (numMetricsSinceLastInterval) {
            currentMetric = {
                .totalFrameTime                             = addValueToMean(currentMetric.totalFrameTime, frametime, numMetricsSinceLastInterval),
                .uptateHooks                                = addValueToMean(currentMetric.uptateHooks, updateHooksTime, numMetricsSinceLastInterval),
                .render                                     = addValueToMean(currentMetric.render, platoRenderTime, numMetricsSinceLastInterval),
                .platorender_beginFrame                     = addValueToMean(currentMetric.platorender_beginFrame, renderer._benchmark_GetBeginFrameTime(), numMetricsSinceLastInterval),
                .platorender_resolveCameraSpaceVertices     = addValueToMean(currentMetric.platorender_resolveCameraSpaceVertices, renderer._benchmark_GetResolveCameraSpaceVerticesTime(), numMetricsSinceLastInterval),
                .platorender_registerTornadoObjects         = addValueToMean(currentMetric.platorender_registerTornadoObjects, renderer._benchmark_RegisterTornadoObjectsTime(), numMetricsSinceLastInterval),
                .tornadorender_beginFrame                   = addValueToMean(currentMetric.tornadorender_beginFrame, renderer._benchmark_GetTornadoBeginFrameTime(), numMetricsSinceLastInterval),
                .tornadorender_render                       = addValueToMean(currentMetric.tornadorender_render, renderer._benchmark_GetTornadoRenderTime(), numMetricsSinceLastInterval),
                .tornadorender_render_perspectiveProjection = addValueToMean(currentMetric.tornadorender_render_perspectiveProjection, renderer._benchmark_GetTornadoRenderPerspectiveProjectionTime(), numMetricsSinceLastInterval),
                .tornadorender_render_cullBackfaces         = addValueToMean(currentMetric.tornadorender_render_cullBackfaces, renderer._benchmark_GetTornadoRenderCullBackfacesTime(), numMetricsSinceLastInterval),
                .tornadorender_render_drawTriangles         = addValueToMean(currentMetric.tornadorender_render_drawTriangles, renderer._benchmark_GetTornadoRenderDrawTrianglesTime(), numMetricsSinceLastInterval),
                .sdlDraw                                    = addValueToMean(currentMetric.sdlDraw, sdlDrawTime, numMetricsSinceLastInterval)
            };

            numMetricsSinceLastInterval++;
        }
        // Else, pupulate it initially
        else {
            currentMetric = {
                .totalFrameTime                             = frametime,
                .uptateHooks                                = updateHooksTime,
                .render                                     = platoRenderTime,
                .platorender_beginFrame                     = renderer._benchmark_GetBeginFrameTime(),
                .platorender_resolveCameraSpaceVertices     = renderer._benchmark_GetResolveCameraSpaceVerticesTime(),
                .platorender_registerTornadoObjects         = renderer._benchmark_RegisterTornadoObjectsTime(),
                .tornadorender_beginFrame                   = renderer._benchmark_GetTornadoBeginFrameTime(),
                .tornadorender_render                       = renderer._benchmark_GetTornadoRenderTime(),
                .tornadorender_render_perspectiveProjection = renderer._benchmark_GetTornadoRenderPerspectiveProjectionTime(),
                .tornadorender_render_cullBackfaces         = renderer._benchmark_GetTornadoRenderCullBackfacesTime(),
                .tornadorender_render_drawTriangles         = renderer._benchmark_GetTornadoRenderDrawTrianglesTime(),
                .sdlDraw                                    = sdlDrawTime,
            };

            numMetricsSinceLastInterval = 1;
        }

        // Capture performance metrics every set time intervals
        if (statsCaptureTimer.GetElapsedTime().AsMilliseconds() >= statsCaptureIntervalMs) {
            statsCaptureTimer.Reset();
            performanceMetrics.push_back(currentMetric);
            numMetricsSinceLastInterval = 0; // This will make the new values to be hardset in the next frame
        }

        // If the current benchmark scene has finished, carry on to the next
        if (!benchmarkScenes[currentBenchmarkSceneIndex]->GetIsRunning()) {

            // Write performance metrics
            const std::string metricsDir = "./dataplotter/performance-metrics";
            const std::string metricsFile = metricsDir + "/" + benchmarkScenes[currentBenchmarkSceneIndex]->GetName() + ".csv";

            std::filesystem::create_directories(metricsDir);
            std::ofstream csvFs(metricsFile, std::ofstream::out);
            if (csvFs.good()) {
                std::stringstream metricsCsvSs;
                std::cout << "Writing performance metrics to " << metricsFile << "..." << std::endl;
                csvFs <<
                    "total frametime,"
                    "updateHooks,"
                    //"platoRender,"
                    "platoRender_beginFrame,"
                    "platoRender_resolveCameraSpaceVertices,"
                    "platoRender_registerTornadoObjects,"
                    "platoRender_tornadoRender_beginFrame,"
                    //"platoRender_tornadoRender_render,"
                    "platoRender_tornadoRender_render_perspectiveProjection,"
                    "platoRender_tornadoRender_render_backfaceCulling,"
                    "platoRender_tornadoRender_render_drawTriangles,"
                    "sdlDraw"
                << std::endl;
                for (const PerformanceMetric& it : performanceMetrics) {
                    csvFs
                        << it.totalFrameTime << ','
                        << it.uptateHooks << ','
                        //<< it.render << ','
                        << it.platorender_beginFrame << ','
                        << it.platorender_resolveCameraSpaceVertices << ','
                        << it.platorender_registerTornadoObjects << ','
                        << it.tornadorender_beginFrame << ','
                        //<< it.tornadorender_render << ','
                        << it.tornadorender_render_perspectiveProjection << ','
                        << it.tornadorender_render_cullBackfaces << ','
                        << it.tornadorender_render_drawTriangles << ','
                        << it.sdlDraw
                        << std::endl;
                }
                csvFs.flush();
                csvFs.close();
            }
            else {
                std::cerr << "Failed to open filestream for write of " << metricsFile << "! Ignoring..." << std::endl;
            }

            // Flush performance metrics
            performanceMetrics.clear();
            performanceMetrics.reserve(3000);
            numMetricsSinceLastInterval = 0;  // Will clear current metric in the next frame

            // Shut down the current benchmark scene
            benchmarkScenes[currentBenchmarkSceneIndex]->Teardown();
            delete benchmarkScenes[currentBenchmarkSceneIndex];
            benchmarkScenes[currentBenchmarkSceneIndex] = nullptr;

            // Delete all world objects and resources
            WorldObjectManager::Free();
            ResourceManager::Free();

            // Unset the current camera in the renderer, as it belongs to the benchmark scene (which just got geleted lol)...
            renderer.SetCamera(nullptr);

            // Initialize the next scene
            currentBenchmarkSceneIndex++;
            if (currentBenchmarkSceneIndex >= benchmarkScenes.size()) {
                Input::Application::Exit();
            }
            else {
                renderWindow.SetWindowTitle(std::string("TornadoPlato Benchmark - ") + benchmarkScenes[currentBenchmarkSceneIndex]->GetName());
                benchmarkScenes[currentBenchmarkSceneIndex]->Start();
            }
        }

        // Only reset the frame timer now to exclude the time it takes
        // to record the new performance metrics struct from the frametimer value
        frametimer.Reset();
    }

    return 0;
}

