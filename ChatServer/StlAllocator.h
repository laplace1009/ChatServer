#pragma once
#include "MemoryHeader.h"
#include "StompAllocator.h"
#include "PoolAllocator.h"

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() = default;

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));

		return reinterpret_cast<T*>(PoolAllocator::Allocate(size));
	}

	void deallocate(T* ptr, size_t size)
	{
		MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

		const int32 allocSize = header->GetAllocSize();

		StompAllocator::Release(header);
	}
};

