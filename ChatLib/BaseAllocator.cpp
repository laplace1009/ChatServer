#include "pch.h"
#include "BaseAllocator.h"

auto BaseAllocator::Allocate(uint64 size) -> void*
{
	return ::malloc(size);
}

auto BaseAllocator::Release(void* ptr) -> void
{
	::free(ptr);
}