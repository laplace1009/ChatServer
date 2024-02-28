#pragma once
#include "Types.h"
#include "MemoryHeader.h"
#include "MemoryPool.h"
#include "TLS.h"

template<uint32 N>
class alignas(16) FixedSizeMemory
{
	enum
	{
		MEMORY_FIXED_SIZE = N,
		MEMORY_POOL_COUNT = 32,
	};

public:
	FixedSizeMemory()
	{
		for (size_t i = 0; i < MEMORY_POOL_COUNT; ++i)
		{
			mPools.emplace_back(new MemoryPool(MEMORY_FIXED_SIZE));
		}
	}
	~FixedSizeMemory() noexcept
	{
		for (auto& pool : mPools)
			delete pool;
		mPools.clear();
	}

	auto Allocate(uint32 size) -> void*
	{
		MemoryHeader* header = nullptr;
		uint32 allocSize = size + sizeof(MemoryHeader);
		if (allocSize > MEMORY_FIXED_SIZE)
			header = new MemoryHeader(allocSize);
		else
			header = mPools[LThreadId]->Pop();

		return MemoryHeader::AttachHeader(header, allocSize);
	}

	auto Release(void* ptr) -> void
	{
		MemoryHeader* header = MemoryHeader::DetachHeader(ptr);
		const uint32 allocSize = header->GetAllocSize();

		ASSERT_CRASH(allocSize > 0);

		if (allocSize > MEMORY_FIXED_SIZE)
			::free(header);
		else
			mPools[LThreadId]->Push(header);
	}

private:
	std::vector<MemoryPool*> mPools;
};