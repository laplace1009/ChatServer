#include "pch.h"
#include "MemoryHeader.h"

MemoryHeader::MemoryHeader(uint32 allocSize)
	: mAllocSize(allocSize)
{
}

auto MemoryHeader::AttachHeader(MemoryHeader* header, uint32 allocSize) -> void*
{
	new(header)MemoryHeader(allocSize);
	return reinterpret_cast<void*>(header + 1);
}

auto MemoryHeader::DetachHeader(void* ptr) -> MemoryHeader*
{
	MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
	return header;
}

auto MemoryHeader::GetAllocSize() -> uint32
{
	return mAllocSize;
}

auto MemoryHeader::SetAllocSize(uint32 allocSize) -> void
{
	mAllocSize = allocSize;
}

