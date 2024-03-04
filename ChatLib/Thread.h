#pragma once
#include <thread>
#include <functional>
#include <vector>

class Thread
{
public:
	Thread();
	~Thread();

	auto Launch(std::function<void(void)> callback) -> void;
	void Join();

private:
	static auto initTLS() -> void;

private:
	Mutex mLock;
	Vector<std::thread> mThreads;
};

