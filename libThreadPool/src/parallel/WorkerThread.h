#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

namespace ettention
{
	class Job;
	class ThreadPool;

    class WorkerThread
    {
    public:
		WorkerThread( ThreadPool* threadPool, size_t threadId );

		virtual ~WorkerThread();

		void workLoop( size_t threadId );

	protected:
		std::thread worker;
		Job* currentJob;
		ThreadPool* threadPool;
		size_t threadId;
		bool busy;

		friend class ThreadPool;
    };
}