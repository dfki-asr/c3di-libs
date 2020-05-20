#include "ThreadPool.h"

#include <algorithm>

#include "WorkerThread.h"
#include "Job.h"

namespace ettention
{
	ThreadPool::ThreadPool( bool usePriority )
		: shouldTerminate(false), usePriority(usePriority)
	{
		unsigned int numberOfThreads = std::thread::hardware_concurrency();
		worker.resize(numberOfThreads);
		for (size_t i = 0; i < worker.size(); i++)
		{
			worker[i] = new WorkerThread( this, (int)i );
		}
		jobCount = 0;
	}

	ThreadPool::~ThreadPool()
	{
		shouldTerminate = true;
		waitForJobsToFinish();
		for (size_t i = 0; i < worker.size(); ++i)
		{
			jobStartCondition.notify_all();
			delete worker[i];
		}
	}

	bool comparePriority( Job* jobA, Job* jobB )
	{
		return jobA->priority < jobB->priority;
	}

	void ThreadPool::enqueue(Job* job)
	{
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			if (usePriority)
			{
				auto position = std::upper_bound( tasks.begin(), tasks.end(), job, &comparePriority );
				tasks.insert(position, job);
				/* for (auto it : tasks) {
					std::cout << std::fixed << std::setprecision(6) << it->priority << ";";
				}
				std::cout << std::endl; */
			}
			else
				tasks.push_back(job);
		}
		jobStartCondition.notify_all();
		jobCount++;
	}

	void ThreadPool::waitForJobsToFinish()
	{
		std::unique_lock<std::mutex> lock(resultMutex);
		while (!allWorkDone())
		{
			jobDoneCondition.wait( lock, [this] { return this->allWorkDone(); } );
		}
	}

	std::deque<Job*>& ThreadPool::getFinishedTasks()
	{
		return finishedTasks;
	}

	void ThreadPool::clearFinishedTasks()
	{
		std::unique_lock<std::mutex> lock( resultMutex );
		while (!finishedTasks.empty())
		{
			delete finishedTasks.back();
			finishedTasks.pop_back();
		}
		jobCount = 0;
	}

	int ThreadPool::numberOfProcessedJobs()
	{
		return jobCount;
	}

	bool ThreadPool::allWorkDone()
	{
		std::unique_lock<std::mutex> lock(queueMutex);

		if (tasks.size() > 0)
			return false;

		for (size_t i = 0; i < worker.size(); i++)
		{
			if (worker[i]->busy)
				return false;
		}

		return true;
	}

}