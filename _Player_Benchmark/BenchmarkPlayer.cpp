#include "BenchmarkPlayer.h"
#include "../Frontend/SDL2RenderWindow.h"
#include "../Plato/EventManager.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/ResourceManager.h"
#include "../Plato/Application.h"
#include "../Plato/Keyboard.h"
#include "../Plato/Clock.h"
#include "BenchmarkScene.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace Plato;

//#define INCLUDE_CUMULATIVE_STATS

namespace {
    const std::string windowBaseTitle = "TornadoPlato Benchmark";
}

BenchmarkPlayer::BenchmarkPlayer(const Vector2i& resolution) :
renderer(resolution, 0, 0)
{
    // Init Plato and RenderWindow as SDL2 render window
    SDL2RenderWindow* sdl2RenderWindow = new SDL2RenderWindow(resolution, windowBaseTitle, renderer.GetPixelBuffer());
    sdl2RenderWindow->EnableMouseCameraControlMode();
    renderWindow = sdl2RenderWindow;


    RegisterPlatoCallbacks();
    Input::EventManager::Init();

    // Instantiate benchmark scenes
    benchmarkScenes = std::vector<BenchmarkScene*>({
        new HighResModelScene(),
        new CaveCamFlightScene()
    });

    // Advance to the first scene
    AdvanceBenchmarkScene();
}

BenchmarkPlayer::~BenchmarkPlayer()
{
    delete renderWindow;
    renderWindow = nullptr;
}

void BenchmarkPlayer::Run()
{
    // Performance metrics variables
    Clock perfTimer;
    Clock statsCaptureTimer;
    constexpr double statsCaptureIntervalMs = 100.0;
    bool justSetVertTrisCount = false;

    // Frametime variables
    Clock frametimer;
    double frametime = 1000.0 / 60.0;

    while (renderWindow->IsRunning()) {
        // Poll the SDL2 windows events
        renderWindow->PollEvents();

        // Digest plato events
        Input::EventManager::Digest();

        // Delete objects that were flagged to be deleted
        WorldObjectManager::DeleteFlaggedObjects();

        // Tick update hooks
        perfTimer.Reset();
        currentBenchmarkScene->Update(frametime);
        WorldObjectManager::CallHook__Update(frametime);
        WorldObjectManager::CallHook__LateUpdate(frametime);
        const double updateHooksTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Render the frame
        perfTimer.Reset();
        renderer.BeginFrame();
        currentBenchmarkScene->Render(&renderer);
        WorldObjectManager::CallHook__Render(&renderer);

        if (!numTris) {
            justSetVertTrisCount = true;
            numTris = renderer.GetNumActiveTris();
            numVertices = renderer.GetNumActiveVertices();
            std::cout << "Scene \"" << currentBenchmarkScene->GetName() << "\" has " << numVertices << " verts and " << numTris << " tris!" << std::endl;
        }

        renderer.Render();
        const double platoRenderTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Display the frame
        perfTimer.Reset();
        renderWindow->RedrawWindow();
        const double sdlDrawTime = perfTimer.GetElapsedTime().AsMilliseconds();

        // Capture how long the frame took to complete
        frametime = frametimer.GetElapsedTime().AsMilliseconds();

        // Digest new metrics
        DigestNewMetrics({
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
        });

        // Capture performance metrics every set time interval
        if (statsCaptureTimer.GetElapsedTime().AsMilliseconds() >= statsCaptureIntervalMs) {
            statsCaptureTimer.Reset();

            // This will make the new values be hard-reset in the next frame
            numMetricsAddedSinceInterval = 0; 

            // Push the new values back
            performanceMetrics.push_back(currentMetric);
        }

        // If the current scene has finished (or the user pressed SPACE), advance to the next scene
        if (!currentBenchmarkScene->GetIsRunning() || Input::Keyboard::GetKeyDown(Input::KEY_CODE::SPACE)) {
            AdvanceBenchmarkScene();
        }

        // If the window title has not yet been set
        if (justSetVertTrisCount) {
            UpdateWindowTitle();
        }

        // Reset the frame timer clock
        frametimer.Reset();
    }
}

void BenchmarkPlayer::RegisterPlatoCallbacks()
{
    RenderWindow* pRenderWindow = renderWindow;

    // Register logic for plato to close the render window
    Input::EventManager::RegisterReverseEventCallback(
        Input::REVERSE_EVENT_CALLBACK::EXIT, 
		[pRenderWindow](const std::vector<double>& params)
		{
            pRenderWindow->Close();
		}
    );
}

bool BenchmarkPlayer::AdvanceBenchmarkScene()
{
    // Delete the current benchmark scene, if defined
    if (currentBenchmarkScene != nullptr) {
        // Dump current scene metrics to csv
        DumpSceneMetrics();

        // Reset scene metrics
        performanceMetrics.clear();
        performanceMetrics.reserve(3000);
        // This will make the new values be hard-reset in the next frame
        numMetricsAddedSinceInterval = 0;

        // Tear the scene down
        currentBenchmarkScene->Teardown();
        delete currentBenchmarkScene;
        currentBenchmarkScene = nullptr;

        // If currentBenchmarkScene is set, the index cant be uninitialized
        benchmarkScenes[currentBenchmarkSceneIndex] = nullptr;

        // Clean up world objects and resources
        WorldObjectManager::Free();
        ResourceManager::Free();

        // Unset the current camera in the renderer, as it belongs to the benchmark scene (which just got deleted lol)...
        renderer.SetCamera(nullptr);

        // Unset vertices/tris count
        numVertices = 0;
        numTris = 0;
    }

    // Handle initialization of the index value
    if (currentBenchmarkSceneIndex == std::string::npos) {
        currentBenchmarkSceneIndex = 0;
    }
    // Else, increment it
    else {
        currentBenchmarkSceneIndex++;
    }

    // Abort if there is no next scene
    if (currentBenchmarkSceneIndex >= benchmarkScenes.size()) {
        Input::Application::Exit();
        return false;
    }

    // Update the scene pointer
    currentBenchmarkScene = benchmarkScenes[currentBenchmarkSceneIndex];

    // Update the window title
    UpdateWindowTitle();

    // Initialize the new scene
    currentBenchmarkScene->Start();

    return true;
}

double BenchmarkPlayer::AddValueToMean(double oldMean, double newVal)
{
    return ((oldMean * (double)numMetricsAddedSinceInterval) + newVal) / (numMetricsAddedSinceInterval + 1);
}

void BenchmarkPlayer::DigestNewMetrics(const PerformanceMetric& newMetric)
{
    // Update current metrics values
    // If it already has values, update its means
    if (numMetricsAddedSinceInterval) {
         currentMetric = {
            .totalFrameTime                             = AddValueToMean(currentMetric.totalFrameTime,                             newMetric.totalFrameTime),
            .uptateHooks                                = AddValueToMean(currentMetric.uptateHooks,                                newMetric.uptateHooks),
            .render                                     = AddValueToMean(currentMetric.render,                                     newMetric.render),
            .platorender_beginFrame                     = AddValueToMean(currentMetric.platorender_beginFrame,                     newMetric.platorender_beginFrame),
            .platorender_resolveCameraSpaceVertices     = AddValueToMean(currentMetric.platorender_resolveCameraSpaceVertices,     newMetric.platorender_resolveCameraSpaceVertices),
            .platorender_registerTornadoObjects         = AddValueToMean(currentMetric.platorender_registerTornadoObjects,         newMetric.platorender_registerTornadoObjects),
            .tornadorender_beginFrame                   = AddValueToMean(currentMetric.tornadorender_beginFrame,                   newMetric.tornadorender_beginFrame),
            .tornadorender_render                       = AddValueToMean(currentMetric.tornadorender_render,                       newMetric.tornadorender_render),
            .tornadorender_render_perspectiveProjection = AddValueToMean(currentMetric.tornadorender_render_perspectiveProjection, newMetric.tornadorender_render_perspectiveProjection),
            .tornadorender_render_cullBackfaces         = AddValueToMean(currentMetric.tornadorender_render_cullBackfaces,         newMetric.tornadorender_render_cullBackfaces),
            .tornadorender_render_drawTriangles         = AddValueToMean(currentMetric.tornadorender_render_drawTriangles,         newMetric.tornadorender_render_drawTriangles),
            .sdlDraw                                    = AddValueToMean(currentMetric.sdlDraw,                                    newMetric.sdlDraw),
        };

        numMetricsAddedSinceInterval++;
    }
    // Else, pupulate it initially
    else {
        currentMetric = newMetric;
        numMetricsAddedSinceInterval = 1;
    }
}

void BenchmarkPlayer::DumpSceneMetrics()
{
    const std::string metricsDir = "./dataplotter/performance-metrics";
    const std::string metricsFile = metricsDir + "/" + currentBenchmarkScene->GetName() + ".csv";

    std::filesystem::create_directories(metricsDir);
    std::ofstream csvFs(metricsFile, std::ofstream::out);
    if (csvFs.good()) {
        std::stringstream metricsCsvSs;
        std::cout << "Writing performance metrics to \"" << metricsFile << "\"..." << std::endl;
        csvFs <<
            "total frametime,"
            "updateHooks,"
            #ifdef INCLUDE_CUMULATIVE_STATS
            "platoRender,"
            #endif
            "platoRender_beginFrame,"
            "platoRender_resolveCameraSpaceVertices,"
            "platoRender_registerTornadoObjects,"
            "platoRender_tornadoRender_beginFrame,"
            #ifdef INCLUDE_CUMULATIVE_STATS
            "platoRender_tornadoRender_render,"
            #endif
            "platoRender_tornadoRender_render_perspectiveProjection,"
            "platoRender_tornadoRender_render_backfaceCulling,"
            "platoRender_tornadoRender_render_drawTriangles,"
            "sdlDraw"
        << std::endl;
        for (const PerformanceMetric& it : performanceMetrics) {
            csvFs
                << it.totalFrameTime << ','
                << it.uptateHooks << ','
                #ifdef INCLUDE_CUMULATIVE_STATS
                << it.render << ','
                #endif
                << it.platorender_beginFrame << ','
                << it.platorender_resolveCameraSpaceVertices << ','
                << it.platorender_registerTornadoObjects << ','
                << it.tornadorender_beginFrame << ','
                #ifdef INCLUDE_CUMULATIVE_STATS
                << it.tornadorender_render << ','
                #endif
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
}

void BenchmarkPlayer::UpdateWindowTitle()
{
    // Add base name
    std::stringstream ss;
    ss << windowBaseTitle;

    // If we have a scene set, add that
    if (currentBenchmarkScene) {
        ss << " - " << currentBenchmarkScene->GetName();
    }

    // If we have a vertices/tris count, add that
    if (numVertices) {
        ss << " (" << numVertices << " verts / " << numTris << " tris)";
    }

    renderWindow->SetWindowTitle(ss.str());
}

