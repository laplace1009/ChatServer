#include "pch.h"
#include "StompAllocator.h"

auto StompAllocator::Allocate(uint64 size) -> void*
{
	const uint64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	// 메모리가 page단위로 할당 되기 때문에 size에 맞게 주소를 할당하기 위한 offset
	const uint64 offset = pageCount * PAGE_SIZE - size;
	void* address = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (address == NULL) CRASH("Failure Memory Allocate");

	return static_cast<void*>(static_cast<BYTE*>(address) + offset);
}

auto StompAllocator::Release(void* ptr) -> void
{
	const int64 address = reinterpret_cast<int64>(ptr);
	// 메모리를 다시 처음 할당 받은 주소로 계산 해줘야 함
	const int64 baseAddress = address - (address % PAGE_SIZE);
	if (::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE) == 0)
		CRASH("Failure Memory Release");
}