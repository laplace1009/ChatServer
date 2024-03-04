#pragma once
#include "ReadWriteLock.h"

template<typename LT>
class ReadLockGuard
{
public:
	explicit ReadLockGuard(LT lock)
		: mLock(lock)
	{
		acquireLock();
	}

	~ReadLockGuard()
	{
		releaseLock();
	}

private:
	auto acquireLock() -> void
	{
		if constexpr (std::is_pointer_v<LT>)
			mLock->ReadLock();
		else
			mLock.ReadLock();
	}

	auto releaseLock() -> void
	{
		if constexpr (std::is_pointer_v<LT>)
			mLock->ReadUnlock();
		else
			mLock.ReadUnlock();
	}

private:
	LT mLock;
};