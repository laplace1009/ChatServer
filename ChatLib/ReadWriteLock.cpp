#include "pch.h"
#include "ReadWriteLock.h"

auto ReadWriteLock::WriteLock() -> void
{
	const uint32 lockOwnerId = (mLockFlag.load() & WRITE_LOCK_MASK) >> 16;

	if (lockOwnerId == LThreadId)
	{
		mWriteLockCount++;
		return;
	}

	const uint32 desired = (LThreadId << 16) & WRITE_LOCK_MASK;

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	while (true)
	{
		for (size_t spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = EMPTY_FLAG;
			if (mLockFlag.compare_exchange_strong(OUT expected, desired))
			{
				mWriteLockCount++;
				return;
			}
		}
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

		std::chrono::duration<double, std::milli> elapsed = end - start;
		if (static_cast<uint32>(elapsed.count()) > MAX_TICK_COUNT) CRASH("Lock Time out");

		std::this_thread::yield();
	}
}

auto ReadWriteLock::WriteUnlock() -> void
{
	ASSERT_CRASH((mLockFlag.load() & WRITE_LOCK_MASK) >> 16 == LThreadId);

	const int32 writeCount = --mWriteLockCount;
	if (writeCount == 0) mLockFlag.store(EMPTY_FLAG);
}

auto ReadWriteLock::ReadLock() -> void
{
	const uint32 lockOwnerId = (mLockFlag.load() & WRITE_LOCK_MASK) >> 16;
	if (lockOwnerId == LThreadId)
	{
		mLockFlag++;
		return;
	}

	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	while (true)
	{
		for (size_t spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = mLockFlag.load() & READ_LOCK_MASK;
			if (mLockFlag.compare_exchange_strong(OUT expected, expected + 1)) return;
		}

		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

		std::chrono::duration<double, std::milli> elapsed = end - start;
		if (static_cast<uint32>(elapsed.count()) > MAX_TICK_COUNT) CRASH("Lock Time out");

		std::this_thread::yield();
	}
}

auto ReadWriteLock::ReadUnlock() -> void
{
	const int32 lockFlag = mLockFlag.fetch_sub(1) & READ_LOCK_MASK;
	if (lockFlag == 0) CRASH('Invalid Unlock');
}
