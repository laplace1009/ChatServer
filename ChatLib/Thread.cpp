#include "pch.h"
#include "Thread.h"
#include "Container.h"

Thread::Thread()
{
	initTLS();
}

Thread::~Thread()
{
	Join();
}

auto Thread::Launch(std::function<void(void)> callback) -> void
{
	LockGuard guard(mLock);
	mThreads.emplace_back(std::thread([=]()
		{
			initTLS();
			callback();
		}));

}

void Thread::Join()
{
	for (std::thread& thread : mThreads)
	{
		if (thread.joinable())
			thread.join();
	}
	mThreads.clear();
}

auto Thread::initTLS() -> void
{
	static Atomic<uint32> SThread = 1;
	LThreadId = SThread.fetch_add(1);
}
