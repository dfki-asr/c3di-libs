#pragma once

namespace ettention
{
	class WorkerThread;

    class Job
    {
    public:
		Job(float priority);
		virtual void performWork() = 0;
		float priority;
		WorkerThread* worker;
    };
}