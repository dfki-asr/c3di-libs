#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <list>

namespace ettention
{
	class Job;
	class WorkerThread;
	class PerThreadState;

    class ThreadPool
    {
    public:
		ThreadPool( bool usePriority );
		virtual ~ThreadPool();

		void enqueue( Job* job ); 
		void waitForJobsToFinish();

		std::deque<Job*>& getFinishedTasks();
		void clearFinishedTasks();

		int numberOfProcessedJobs();

	protected:
		bool allWorkDone();

	public:
		std::vector<WorkerThread*> worker;
		std::list<Job*> tasks;
		std::deque<Job*> finishedTasks;

		std::mutex outputMutex;
		std::mutex queueMutex;
		std::mutex resultMutex;
		std::condition_variable jobStartCondition;
		std::condition_variable jobDoneCondition;
		bool usePriority;
		int jobCount;

		bool shouldTerminate;

		friend class WorkerThread;
    };
}