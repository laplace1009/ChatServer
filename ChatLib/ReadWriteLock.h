#pragma once
#include "Types.h"
#include <atomic>
#include <thread>

class ReadWriteLock
{
	// 필요한 상수들
	enum : uint32
	{
		MAX_SPIN_COUNT = 4000,
		MAX_TICK_COUNT = 10000,
		EMPTY_FLAG = 0x0000'0000,
		WRITE_LOCK_MASK = 0xFFFF'0000,
		READ_LOCK_MASK = 0x0000'FFFF,
	};

public:
	auto WriteLock() -> void;
	auto WriteUnlock() -> void;
	auto ReadLock() -> void;
	auto ReadUnlock() -> void;

private:
	Atomic<uint32> mLockFlag = EMPTY_FLAG;
	uint32 mWriteLockCount = 0;
};

