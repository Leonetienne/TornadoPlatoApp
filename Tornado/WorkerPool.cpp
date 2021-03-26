#include "WorkerPool.h"
#include "CrossPlatformSyscalls.h"

WorkerPool::WorkerPool(std::size_t numWorkers)
{
	workers.reserve(numWorkers);

	for (std::size_t i = 0; i < numWorkers; i++)
	{
		Worker* newWorker = new Worker();
		newWorker->ownThread = new std::thread(&Worker::Lifecycle, newWorker);

		workers.push_back(newWorker);
	}

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
		if (wt->state == WorkerTaskState::QUEUED)
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
		for (Worker* w : workers)
			if (w->IsIdling()) // And if a worker is not working
				for (WorkerTask* wt : taskQueue)
					if (wt->state == WorkerTaskState::QUEUED) // And if a task is unassigned
						w->DoTask(wt);	// Add that task
	
		Sleep(16); // Reduce cpu overhead
	}
	
	// Now all tasks are assigned. Wait for them to finish
	for (const Worker* w : workers)
		while (!w->IsIdling())
			Sleep(16);
	
	// Now all tasks are finished. Let's clean up after ourselves!
	for (WorkerTask* wt : taskQueue)
		delete wt;
	taskQueue.clear();

	return;
}

////////////////////////////////////////////////////////

Worker::Worker()
{
	Lifecycle();
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
	this->task->state = WorkerTaskState::ASSIGNED;
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
		// Begin working on new task
		if ((doTask) && (task != nullptr))
		{
			// Set flags for working
			doTask = false;
			isIdling = false;
			task->state = WorkerTaskState::COMPUTING;

			// Run task
			task->task();

			// Set flags for idling
			isIdling = true;
			task->state = WorkerTaskState::FINISHED;
		}
		// Idling
		else
		{
			Sleep(16); // Reduce cpu load drastically when doing 'nothing'.
		}
	}

	return;
}
