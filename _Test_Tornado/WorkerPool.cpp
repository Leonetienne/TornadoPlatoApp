#include "CppUnitTest.h"
#include "../Tornado/WorkerPool.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>
#include <chrono>
#include <math.h>

#define SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA (atan2(sqrt(sqrt(sin((rng()%21929) / 1.29422) / ((rng()%3292) / 0.0028))), pow(22.1, 3.14159265358979323846)))

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Multithreading
{
	TEST_CLASS(_WorkerPool)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_WorkerPool()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests if the method GetNumWorkers() matches the amount of workers passed to the constructor
		TEST_METHOD(Does_WorkerCount_Match)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				// Create worker pool
				int num_threads = rng() % 16 + 1;
				WorkerPool pool(num_threads);

				Assert::AreEqual(num_threads, (int)pool.GetNumWorkers());
			}

			return;
		}

		// Tests whether the method GetQueueSize() matches the amount of tasks queued
		TEST_METHOD(Does_Queuesize_Match)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				// Create worker pool
				int num_tasks = rng() % 16 + 1;
				WorkerPool pool(8);

				for (std::size_t j = 0; j < num_tasks; j++)
					pool.QueueTask(new WorkerTask);

				Assert::AreEqual(num_tasks, (int)pool.GetQueueLength());
			}

			return;
		}


		void ExampleTask__FillPixels(uint8_t* begin, uint8_t* end)
		{
			for (uint8_t* i = begin; i < end; i++)
				*i = 255;

			return;
		}
		void ExampleTask__FillPixels_ComputingIntensive(uint8_t* begin, uint8_t* end)
		{
			for (uint8_t* i = begin; i < end; i++)
				*i = (uint8_t)SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA;

			return;
		}

		// Does the worker pool actually complete work?
		TEST_METHOD(Does_Complete_Tasks)
		{
			// Create something to do
			constexpr std::size_t numThreads = 8;
			constexpr std::size_t numPixels = 512*512;
			uint8_t* pixels = new uint8_t[numPixels];
			memset(pixels, 0, numPixels);
			
			// Create worker pool
			WorkerPool pool(numThreads);

			// Create and queue tasks
			constexpr std::size_t slice_size = (numPixels) / 8;
			for (std::size_t i = 0; i < numThreads; i++)
			{
				WorkerTask* newTask = new WorkerTask;
				newTask->task = std::bind(
					&_WorkerPool::ExampleTask__FillPixels, this,
					pixels + (i * slice_size),
					pixels + ((i + 1) * slice_size)
				);

				pool.QueueTask(newTask);
			}

			// Execute tasks
			pool.Execute();

			// Check that the workers did their work
			for (std::size_t i = 0; i < numPixels; i++)
				Assert::AreEqual(255, (int)pixels[i]);

			// Cleanup
			delete[] pixels;

			return;
		}

		// Does the worker pool actually complete work, even if there are MUCH more tasks than workers?
		TEST_METHOD(Does_Complete_Tasks_A_LOT_Of_Tasks)
		{
			// Create something to do
			constexpr std::size_t numThreads = 8;
			constexpr std::size_t numPixels = 512 * 512;
			uint8_t* pixels = new uint8_t[numPixels];
			memset(pixels, 0, numPixels);

			// Create worker pool
			WorkerPool pool(numThreads);

			// Create and queue tasks
			constexpr std::size_t slice_size = (numPixels) / (numThreads*64);
			for (std::size_t i = 0; i < numThreads*64; i++)
			{
				WorkerTask* newTask = new WorkerTask;
				newTask->task = std::bind(
					&_WorkerPool::ExampleTask__FillPixels, this,
					pixels + (i * slice_size),
					pixels + ((i + 1) * slice_size)
				);

				pool.QueueTask(newTask);
			}

			// Execute tasks
			pool.Execute();

			// Check that the workers did their work
			for (std::size_t i = 0; i < numPixels; i++)
				Assert::AreEqual(255, (int)pixels[i]);

			// Cleanup
			delete[] pixels;

			return;
		}

		// Tests that filling a pixel buffer via workers is faster than without
		TEST_METHOD(Is_Faster_Than_Without)
		{
			// Create something to do
			constexpr std::size_t numPixels = 4096*4096;
			uint8_t* pixels = new uint8_t[numPixels];
			memset(pixels, 0, numPixels);

			// Benchmark single thread
			std::chrono::nanoseconds single;
			{
				uint8_t* pixelsEnd = pixels + numPixels;
				auto begin = std::chrono::high_resolution_clock::now();

				for (uint8_t* i = pixels; i < pixelsEnd; i++)
					*i = (uint8_t)SUPER_COMPUTATIONALLY_EXPENSIVE_FORMULA;

				auto end = std::chrono::high_resolution_clock::now();
				single = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(end - begin));
			}

			// Benchmark worker pool
			// Create worker pool
			std::chrono::nanoseconds multi;
			{
				constexpr std::size_t numThreads = 24;
				WorkerPool pool(numThreads); // Would be initialized beforehand

				auto begin = std::chrono::high_resolution_clock::now();

				// Create and queue tasks
				constexpr std::size_t slice_size = (numPixels) / numThreads;
				for (std::size_t i = 0; i < numThreads; i++)
				{
					WorkerTask* newTask = new WorkerTask;
					newTask->task = std::bind(
						&_WorkerPool::ExampleTask__FillPixels_ComputingIntensive, this,
						pixels + (i * slice_size),
						pixels + ((i + 1) * slice_size)
					);

					pool.QueueTask(newTask);
				}

				// Execute tasks
				pool.Execute();

				auto end = std::chrono::high_resolution_clock::now();
				multi = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(end - begin));
			}

			std::wstringstream wss;
			wss << L"Wtf, single threaded was faster than multithreaded! Single: " << single.count() << L"ns | Multi: " << multi.count() << L"ns" << std::endl;
			Assert::IsTrue(multi < single, wss.str().c_str());

			return;
		}

		// =========== MEMORY LEAK TESTS ===========
		// These tests depends on debug-mode for memory insights.
		// Thus, they only works in debug mode.
		#ifdef _DEBUG

		// Tests for the worker pool to not leak any memory when doing absolutely nothing
		TEST_METHOD(No_Memoryleak_Empty)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				WorkerPool pool(24);
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests for the worker pool to not leak any memory when doing work
		TEST_METHOD(No_Memoryleak_Working)
		{
			// Preparation
			constexpr std::size_t numPixels = 512 * 512;
			uint8_t* pixels = new uint8_t[numPixels];
			memset(pixels, 0, numPixels);

			MemoryLeakDetector mld;
			mld.Init();
			{
				constexpr std::size_t numThreads = 8;
				WorkerPool pool(numThreads);

				// Create and queue tasks
				constexpr std::size_t slice_size = (numPixels) / (numThreads * 64);
				for (std::size_t i = 0; i < numThreads * 64; i++)
				{
					WorkerTask* newTask = new WorkerTask;
					newTask->task = std::bind(
						&_WorkerPool::ExampleTask__FillPixels, this,
						pixels + (i * slice_size),
						pixels + ((i + 1) * slice_size)
					);

					pool.QueueTask(newTask);
				}

				// Execute tasks
				pool.Execute();

				// Check that the workers did their work
				for (std::size_t i = 0; i < numPixels; i++)
					Assert::AreEqual(255, (int)pixels[i]);
			}

			Assert::IsFalse(mld.DetectLeak());

			// Cleanup
			delete[] pixels;

			return;
		}

		#endif
	};
}
