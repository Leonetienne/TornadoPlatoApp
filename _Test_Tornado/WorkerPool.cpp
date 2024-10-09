
#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../Tornado/WorkerPool.h"
#include <random>
#include <chrono>
#include <cmath>
#include <thread>

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

#define SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA (atan2(sqrt(sqrt(sin((rng() % 21929) / 1.29422) / ((rng() % 3292) / 0.0028))), pow(22.1, 3.14159265358979323846)))

using namespace TorGL;

TEST_CASE("_WorkerPool Tests", "[multithreading]") {
    std::mt19937 rng((std::random_device())());

    SECTION("WorkerCount_0_Yields_SystemThread_Count_Workers") {
        WorkerPool wp(0);
        REQUIRE(wp.GetNumWorkers() == std::thread::hardware_concurrency());
    }

    SECTION("Does_WorkerCount_Match") {
        // Test 10 times
        for (std::size_t i = 0; i < 10; i++) {
            int num_threads = rng() % 16 + 1;
            WorkerPool pool(num_threads);
            REQUIRE(pool.GetNumWorkers() == num_threads);
        }
    }

    SECTION("Does_Queuesize_Match") {
        // Test 10 times
        for (std::size_t i = 0; i < 10; i++) {
            int num_tasks = rng() % 16 + 1;
            WorkerPool pool(8);

            for (std::size_t j = 0; j < num_tasks; j++)
                pool.QueueTask(new WorkerTask);

            REQUIRE(pool.GetQueueLength() == num_tasks);
        }
    }

    auto ExampleTask__FillPixels = [](uint8_t* begin, uint8_t* end) {
        for (uint8_t* i = begin; i < end; i++)
            *i = 255;
    };

    auto ExampleTask__FillPixels_ComputingIntensive = [](uint8_t* begin, uint8_t* end, std::mt19937& rng) {
        for (uint8_t* i = begin; i < end; i++)
            *i = (uint8_t)SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA;
    };

    SECTION("Does_Complete_Tasks") {
        constexpr std::size_t numThreads = 8;
        constexpr std::size_t numPixels = 512 * 512;
        uint8_t* pixels = new uint8_t[numPixels];
        memset(pixels, 0, numPixels);

        WorkerPool pool(numThreads);

        constexpr std::size_t slice_size = numPixels / numThreads;
        for (std::size_t i = 0; i < numThreads; i++) {
            WorkerTask* newTask = new WorkerTask;
            newTask->task = std::bind(ExampleTask__FillPixels, pixels + (i * slice_size), pixels + ((i + 1) * slice_size));
            pool.QueueTask(newTask);
        }

        pool.Execute();

        for (std::size_t i = 0; i < numPixels; i++) {
            REQUIRE(pixels[i] == 255);
        }

        delete[] pixels;
    }

    SECTION("Does_Complete_Tasks_A_LOT_Of_Tasks") {
        constexpr std::size_t numThreads = 8;
        constexpr std::size_t numPixels = 512 * 512;
        uint8_t* pixels = new uint8_t[numPixels];
        memset(pixels, 0, numPixels);

        WorkerPool pool(numThreads);

        constexpr std::size_t slice_size = numPixels / (numThreads * 64);
        for (std::size_t i = 0; i < numThreads * 64; i++) {
            WorkerTask* newTask = new WorkerTask;
            newTask->task = std::bind(ExampleTask__FillPixels, pixels + (i * slice_size), pixels + ((i + 1) * slice_size));
            pool.QueueTask(newTask);
        }

        pool.Execute();

        for (std::size_t i = 0; i < numPixels; i++) {
            REQUIRE(pixels[i] == 255);
        }

        delete[] pixels;
    }

    SECTION("Is_Faster_Than_Without") {
        constexpr std::size_t numPixels = 4096 * 4096;
        uint8_t* pixels = new uint8_t[numPixels];
        memset(pixels, 0, numPixels);

        std::chrono::nanoseconds single;
        {
            uint8_t* pixelsEnd = pixels + numPixels;
            auto begin = std::chrono::high_resolution_clock::now();

            for (uint8_t* i = pixels; i < pixelsEnd; i++)
                *i = (uint8_t)SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA;

            auto end = std::chrono::high_resolution_clock::now();
            single = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        }

        std::chrono::nanoseconds multi;
        {
            constexpr std::size_t numThreads = 24;
            WorkerPool pool(numThreads);

            auto begin = std::chrono::high_resolution_clock::now();

            constexpr std::size_t slice_size = numPixels / numThreads;
            for (std::size_t i = 0; i < numThreads; i++) {
                WorkerTask* newTask = new WorkerTask;
                newTask->task = std::bind(ExampleTask__FillPixels_ComputingIntensive, pixels + (i * slice_size), pixels + ((i + 1) * slice_size), rng);
                pool.QueueTask(newTask);
            }

            pool.Execute();

            auto end = std::chrono::high_resolution_clock::now();
            multi = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        }

        REQUIRE(multi < single);

        delete[] pixels;
    }
}

#undef SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA

