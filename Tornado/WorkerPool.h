#pragma once
#include <functional>
#include <cstddef>
#include <vector>
#include <thread>

enum class WorkerTaskState
{
	QUEUED,
	ASSIGNED, // This is an intermediate state that should not last longer than a few cycles
	COMPUTING,
	FINISHED
};

struct WorkerTask
{
	std::function<void()> task;
	WorkerTaskState state = WorkerTaskState::QUEUED;
};

class WorkerPool;

class Worker
{
public:
	Worker();
	~Worker();

	// Will attempt to start working on a provided task
	// Will fail if the worker is already working on a different task
	bool DoTask(WorkerTask* task);

	// Will return whether or not the worker is currently working
	bool IsIdling() const;

	// Will instruct the worker to exit his THREAD!! (not the task) as soon as its current task finishes
	void Stop();


private:
	void Lifecycle();
	
	std::thread* ownThread;
	WorkerTask* task = nullptr;
	bool isIdling = true;
	bool doTask = false;
	bool doStop = false;

	friend WorkerPool;
};

class WorkerPool
{
public:
	WorkerPool(std::size_t numWorkers);

	// Will queue a task to be done
	void QueueTask(WorkerTask* task);

	// Will compute all queued tasks and return when they are finished
	void Execute();

	// Will return the amound of unassigned tasks
	std::size_t GetQueueLength() const;

	// Will return the total amount of workers
	std::size_t GetNumWorkers() const;

	// Will return the number of workers currently chewing on a task
	std::size_t GetNumActiveWorkers() const;

private:
	std::vector<Worker*> workers;
	std::vector<WorkerTask*> taskQueue;
};

