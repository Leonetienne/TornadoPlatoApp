#pragma once
#include <functional>
#include <cstddef>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace TorGL
{
	/** Trivial data structure representing a parallelizable task
	*/
	struct WorkerTask
	{
		//! Task states
		enum class State
		{
			QUEUED,
			ASSIGNED, // This is an intermediate state that should not last longer than a few cycles
			COMPUTING,
			FINISHED
		};

		//! A "function pointer" to the task method
		std::function<void()> task;

		//! The current state of the task
		State state = State::QUEUED;
	};

	class WorkerPool;

	/** A worker representing a cpu thread
	*/
	class Worker
	{
	public:
		~Worker();

		//! Will attempt to start working on a provided WorkerTask. 
		//! Will fail if the worker is already working on a different WorkerTask.  
		//! This does not wake the worker up! `notify_all()` still needs to be called before it actually starts working!
		bool DoTask(WorkerTask* task);

		//! Will return whether or not the worker is currently working
		bool IsIdling() const;

		//! Will instruct the Worker to exit his THREAD!! (not the task) as soon as its current WorkerTask finishes
		void Stop();

		// These are needed for unit testing (without a worker pool)
		#ifdef _DEBUG
		Worker(WorkerPool* pool);
		void SetThread(std::thread* thr) { ownThread = thr; };
		std::thread* GetThread() { return ownThread; };
		void Lifecycle();
		#endif;

	private:
		#ifndef _DEBUG
		// Disallow public instanciation
		Worker(WorkerPool* pool);
		void Lifecycle();
		#endif

		std::thread* ownThread = nullptr;
		WorkerTask* task = nullptr;
		WorkerPool* pool;
		bool isIdling = true;
		bool doTask = false;
		bool doStop = false;

		friend class WorkerPool;
	};

	/** A worker pool (or thread pool) responsible for scheduling parallelizable over \f$n\f$ cpu cores.
	* This utility follows this workflow:
	*	1. Queue a lot of WorkerTask structs
	*	2. Execute();
	*	3. Tasks are done.
	*
	* This thread pool makes use of an `std::condition_variable`, meaning the threads are sleeping when there is no work to do.
	*/
	class WorkerPool
	{
	public:
		WorkerPool(std::size_t numWorkers);
		~WorkerPool();

		//! Will queue a task to be done
		void QueueTask(WorkerTask* task);

		//! Will compute all queued tasks and return when they are finished
		void Execute();

		//! Will return the amound of unassigned tasks
		std::size_t GetQueueLength() const;

		//! Will return the total amount of workers
		std::size_t GetNumWorkers() const;

		//! Will return the number of workers currently chewing on a task
		std::size_t GetNumActiveWorkers() const;

	private:
		std::vector<Worker*> workers;
		std::vector<WorkerTask*> taskQueue;

		std::mutex mutex;
		std::condition_variable conditional_lock;

		friend class Worker;
	};
}
