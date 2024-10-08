#include "WorkerPool.h"
#include "CrossPlatformSyscalls.h"

using namespace TorGL;

WorkerPool::WorkerPool(std::size_t numWorkers)
{
    // If number of workers is 0 (default argument for tornado and plato renderers) determine automatically
	if (numWorkers == 0) {
        numWorkers = std::thread::hardware_concurrency();

        // hardware_concurrency() may return 0, if it does, just set it to 1
        if (numWorkers == 0) {
            numWorkers = 1;
        }
    }

	workers.reserve(numWorkers);

	for (std::size_t i = 0; i < numWorkers; i++)
	{
		Worker* newWorker = new Worker(this);
		newWorker->ownThread = new std::thread(&Worker::Lifecycle, newWorker);

		workers.push_back(newWorker);
	}

	return;
}

WorkerPool::~WorkerPool()
{
	// Stop all workers
	for (Worker* w : workers)
		w->Stop();

	// Wake up the workers for them to notice the stop signal
	{
		std::unique_lock<std::mutex> lck(mutex);
		conditional_lock.notify_all();
	}

	// Wait for all workers to finish
	for (const Worker* w : workers)
		w->ownThread->join();

	// Free all workers
	for (Worker* w : workers)
		delete w;
	workers.clear();
	
	// Free all tasks
	for (WorkerTask* wt : taskQueue)
		delete wt;
	taskQueue.clear();

	return;
}

void WorkerPool::QueueTask(WorkerTask* task)
{
	taskQueue.push_back(task);

	return;
}

std::size_t WorkerPool::GetQueueLength() const
{
	std::size_t count = 0;

	for (const WorkerTask* wt : taskQueue)
		if (wt->state == WorkerTask::State::QUEUED)
			count++;

	return count;
}

std::size_t WorkerPool::GetNumWorkers() const
{
	return workers.size();
}

std::size_t WorkerPool::GetNumActiveWorkers() const
{
	std::size_t count = 0;

	for (const Worker* w : workers)
		if (!w->IsIdling())
			count++;

	return count;
}

void WorkerPool::Execute()
{
	// As long as we still have unassigned tasks to do
	while (GetQueueLength() > 0)
	{
		bool didQueueAnything = false;

		for (Worker* w : workers)
			if (w->IsIdling()) // And if a worker is not working
				for (WorkerTask* wt : taskQueue)
					if (wt->state == WorkerTask::State::QUEUED) // And if a task is unassigned
					{
						w->DoTask(wt);	// Add that task
						didQueueAnything = true;
						
					}

		// If we queued a new task, we should notify the threads about it
		if (didQueueAnything)
		{
			std::unique_lock<std::mutex> lck(mutex);
			conditional_lock.notify_all();
		}

		//cpSleep(0); // Reduce cpu overhead
	}
	
	// Now all tasks are assigned. Wait for them to finish
	for (const Worker* w : workers)
		while (!w->IsIdling())
			cpSleep(0);
	
	// Now all tasks are finished. Let's clean up after ourselves!
	for (WorkerTask* wt : taskQueue)
		delete wt;
	taskQueue.clear();

	return;
}

////////////////////////////////////////////////////////

Worker::Worker(WorkerPool* pool)
	:
	pool {pool}
{
	return;
}

Worker::~Worker()
{
	delete ownThread;
	ownThread = nullptr;
	return;
}

bool Worker::DoTask(WorkerTask* task)
{
	// Fail, if already working
	if (!isIdling)
		return false;

	this->task = task;
	this->task->state = WorkerTask::State::ASSIGNED;
	isIdling = false;
	doTask = true;

	return true;
}

bool Worker::IsIdling() const
{
	return isIdling;
}

void Worker::Stop()
{
	doStop = true;
	return;
}

void Worker::Lifecycle()
{
	while (!doStop)
	{
		// Idle whilst waiting for either a stop, or a go signal
		while ((!doTask) && (!doStop))
		{
			std::unique_lock<std::mutex> lck(pool->mutex);
			pool->conditional_lock.wait(lck, [this] { return (doTask || doStop); });
			//cpSleep(0);
		}
		
		// Something happened!
		// Was it a stop signal?
		if (doStop)
			break;

		// Or should we continue?
		if (task != nullptr)
		{
			// Set flags for working
			doTask = false;
			isIdling = false;
			task->state = WorkerTask::State::COMPUTING;

			// Run task
			task->task();

			// Set flags for idling
			isIdling = true;
			task->state = WorkerTask::State::FINISHED;
		}
	}

	return;
}
