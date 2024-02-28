#include "pch.h"
#include "MultiSizeMemory.h"

MultiSizeMemory::MultiSizeMemory()
{
	uint32 size = 0;
	uint32 tableIdx = 0;
	for (size = 32; size <= ALLOCATE_SIZE; size += 32)
	{
		mPools.emplace_back(new MemoryPool(size));
		while (tableIdx <= size)
			mPoolTable[tableIdx++] = mPools.back();
	}

	for (; size <= ALLOCATE_SIZE << 1; size += 64)
	{
		mPools.emplace_back(new MemoryPool(size));
		while (tableIdx <= size)
			mPoolTable[tableIdx++] = mPools.back();
	}

	for (; size <= ALLOCATE_SIZE << 2; size += 128)
	{
		mPools.emplace_back(new MemoryPool(size));
		while (tableIdx <= size)
			mPoolTable[tableIdx++] = mPools.back();
	}

	for (; size <= ALLOCATE_SIZE << 3; size += 256)
	{
		mPools.emplace_back(new MemoryPool(size));
		while (tableIdx <= size)
			mPoolTable[tableIdx++] = mPools.back();
	}
}

MultiSizeMemory::~MultiSizeMemory() noexcept
{
	for (MemoryPool* pool : mPools)
		delete pool;

	mPools.clear();
}

auto MultiSizeMemory::Allocate(uint32 size) -> void*
{
	MemoryHeader* header = nullptr;
	const uint32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Allocate(allocSize));
#else
	if (allocSize > MAX_ALLOCATE_SIZE)
		header = reinterpret_cast<MemoryHeader*>(::malloc(allocSize));
	else
		header = mPoolTable[allocSize]->Pop();

#endif
	return MemoryHeader::AttachHeader(header, allocSize);
}

auto MultiSizeMemory::Release(void* ptr) -> void
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const uint32 allocSize = header->GetAllocSize();
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOCATE_SIZE)
		::free(header);
	else
		mPoolTable[allocSize]->Push(header);
#endif
}
