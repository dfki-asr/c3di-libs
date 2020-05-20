#include "WorkerThread.h"
#include "ThreadPool.h"
#include "Job.h"

namespace ettention
{

	WorkerThread::WorkerThread(ThreadPool* threadPool, size_t threadId) 
		: threadPool(threadPool)
		, threadId(threadId)
		, busy(false)
	{
		worker = std::thread(&WorkerThread::workLoop, this, threadId);
	}

	WorkerThread::~WorkerThread()
	{
		worker.join();
	}

	void WorkerThread::workLoop(size_t threadId)
	{
		while (!threadPool->shouldTerminate)
		{
			{
				std::unique_lock<std::mutex> lk(threadPool->queueMutex);

				while (!threadPool->shouldTerminate && threadPool->tasks.size() == 0)
				{
					threadPool->jobStartCondition.wait(lk);
				}

				if (threadPool->shouldTerminate)
					return;

				currentJob = threadPool->tasks.front();
				threadPool->tasks.pop_front();

				busy = true;
			}

			currentJob->worker = this;
			currentJob->performWork();
			
			busy = false;

			{
				std::unique_lock<std::mutex> lk(threadPool->resultMutex);
				threadPool->finishedTasks.push_front( currentJob );
				threadPool->jobDoneCondition.notify_one();
			}

		}
	}

}