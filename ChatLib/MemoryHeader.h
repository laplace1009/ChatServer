#pragma once
#include "Types.h"
#include "Macro.h"
#include <new>

// Debug ¿ë
class MemoryHeader
{
public:
	MemoryHeader(uint32 allocSize);
	static auto AttachHeader(MemoryHeader* header, uint32 allocSize) -> void*;
	static auto DetachHeader(void* ptr) -> MemoryHeader*;
	auto GetAllocSize() -> uint32;
	auto SetAllocSize(uint32 allocSize) -> void;

private:
	uint32 mAllocSize;
};

