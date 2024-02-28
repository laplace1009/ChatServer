#pragma once
#include "Types.h"
#include "MemoryHeader.h"
#include "MemoryPool.h"
#include <array>

class alignas(16) MultiSizeMemory
{
	enum
	{
		POOL_COUNT = (512 / 32) + (1024 / 64) + (2048 / 128) + (4096 / 256),
		ALLOCATE_SIZE = 512,
		MAX_ALLOCATE_SIZE = 4096,
	};

public:
	MultiSizeMemory();
	~MultiSizeMemory() noexcept;

	auto Allocate(uint32 size) -> void*;
	auto Release(void* ptr) -> void;

private:
	std::vector<MemoryPool*> mPools;
	std::array<MemoryPool*, MAX_ALLOCATE_SIZE + 1> mPoolTable{ nullptr };
};

