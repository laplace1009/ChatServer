#include "pch.h"
#include "PoolAllocator.h"
#include "MultiSizeMemory.h"

auto PoolAllocator::Allocate(int32 size) -> void*
{
    return GMemory->Allocate(size);
}

auto PoolAllocator::Release(void* ptr) -> void
{
    GMemory->Release(ptr);
}
