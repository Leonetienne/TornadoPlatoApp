#include "CppUnitTest.h"
#include "../Tornado/WorkerPool.h"
#include "../Tornado/CrossPlatformSyscalls.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Multithreading
{
	TEST_CLASS(_Worker)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Worker()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		#ifdef _DEBUG
		void Example_Task(double x, double y, double z, double* out_sqrmag)
		{
			cpSleep(500);
			*out_sqrmag = x*x + y*y + z*z;
			return;
		}

		// Tests that a worker can do a task
		TEST_METHOD(Can_Do_Task)
		{
			// Create task
			WorkerTask task;
			
			double x = 33;
			double y = 44;
			double z = 55;
			double sqrmag;
			
			task.task = std::bind(&_Worker::Example_Task, this, x, y, z, &sqrmag);
			
			// Assign task to worker
			Worker* worker = new Worker;
			worker->SetThread(new std::thread(&Worker::Lifecycle, worker));
			worker->DoTask(&task);
			
			// Wait for worker
			while (!worker->IsIdling());
			
			// Check that the task did our calculation
			Assert::AreEqual(6050.0, sqrmag);

			// Cleanup
			worker->Stop();
			worker->GetThread()->join();
			delete worker;
		
			return;
		}

		// Tests that a tasks states are correct
		TEST_METHOD(Task_States_Are_Correct)
		{
			// Create task
			WorkerTask task;

			// Check that task state is "queued"
			Assert::IsTrue(WorkerTaskState::QUEUED == task.state, L"Failed at QUEUED.");

			double x = 33;
			double y = 44;
			double z = 55;
			double sqrmag;

			task.task = std::bind(&_Worker::Example_Task, this, x, y, z, &sqrmag);

			// Assign task to worker
			Worker* worker = new Worker;
			worker->SetThread(new std::thread(&Worker::Lifecycle, worker));
			worker->DoTask(&task);

			// Check that task state is "ASSIGNED" (task sleeps for 500ms)
			Assert::IsTrue(WorkerTaskState::ASSIGNED == task.state, L"Failed at ASSIGNED.");

			cpSleep(50);
			// Check that task state is "COMPUTED" (task sleeps for 500ms)
			Assert::IsTrue(WorkerTaskState::COMPUTING == task.state, L"Failed at COMPUTING.");

			// Wait for worker
			while (!worker->IsIdling());

			// Check that task state is "finished"
			Assert::IsTrue(WorkerTaskState::FINISHED == task.state, L"Failed at FINISHED.");

			// Cleanup
			worker->Stop();
			worker->GetThread()->join();
			delete worker;

			return;
		}

		// Tests that executing a worker task does not lead to a memory leak
		TEST_METHOD(No_Memory_Leak)
		{
			MemoryLeakDetector mld;
			mld.Init();
			
			{
				// Create task
				WorkerTask task;

				double x = 33;
				double y = 44;
				double z = 55;
				double sqrmag;

				task.task = std::bind(&_Worker::Example_Task, this, x, y, z, &sqrmag);

				// Assign task to worker
				Worker* worker = new Worker;
				worker->SetThread(new std::thread(&Worker::Lifecycle, worker));
				worker->DoTask(&task);

				// Wait for worker
				while (!worker->IsIdling());

				// Cleanup
				worker->Stop();
				worker->GetThread()->join();
				delete worker;
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		#endif
	};
}
