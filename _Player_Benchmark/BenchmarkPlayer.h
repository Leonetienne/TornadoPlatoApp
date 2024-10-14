#define _BENCHMARK_CONTEXT
#include "../Plato/Renderer.h"
#include "RenderWindow.h"
#include "PerformanceMetric.h"
#include "BenchmarkScene.h"
#include "CaveCamFlightScene.h"
#include "HighResModelScene.h"
#include <vector>

class BenchmarkPlayer
{
    public:
        BenchmarkPlayer(const Plato::Vector2i& resolution);

        // Run the main app
        void Run();

    private:
        // Will switch to the next benchmark scene in queue
        bool AdvanceBenchmarkScene();

        void RegisterPlatoCallbacks();

        // Will dump current scene metrics to csv
        void DumpSceneMetrics();

        void DigestNewMetrics(const PerformanceMetric& newMetric);

        // Will first transform the mean value back to the average sum of individual values, then add a new value, and perform the mean-division again
        double AddValueToMean(double oldMean, double newVal);


        Renderer renderer;
        RenderWindow renderWindow;
        std::vector<BenchmarkScene*> benchmarkScenes;
        std::size_t currentBenchmarkSceneIndex = std::string::npos;
        BenchmarkScene* currentBenchmarkScene = nullptr;
        std::vector<PerformanceMetric> performanceMetrics;
        PerformanceMetric currentMetric;
        // How many performance metrics have been averaged since the last insertion into performanceMetrics
        std::size_t numMetricsAddedSinceInterval = 0;
};

