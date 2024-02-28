#pragma once
#include "ReadWriteLock.h"

template<typename LT>
class WriteLockGuard
{
public:
	explicit WriteLockGuard(LT lock)
		: mLock(lock)
	{
		acquireLock();
	}

	~WriteLockGuard()
	{
		releaseLock();
	}

private:
	auto acquireLock() -> void
	{
		if constexpr (std::is_pointer_v<LT>)
			mLock->WriteLock();
		else
			mLock.WriteLock();
	}

	auto releaseLock() -> void
	{
		if constexpr (std::is_pointer_v<LT>)
			mLock->WriteUnlock();
		else
			mLock.WriteUnlock();
	}

private:
	LT mLock;
};