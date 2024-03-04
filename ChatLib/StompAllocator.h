#pragma once
#include "Types.h"
#include "Macro.h"
#include <Windows.h>

class StompAllocator
{
	// 기본적으로 os에서 프로세스에 메모리를 할당하는 단위(page)는 4kb
	// Windows VirtualAlloc은 메모리를 페이지 단위로 할당하기 때문
	static constexpr uint64 PAGE_SIZE = 0x1000;

public:
	static auto Allocate(uint64 size) -> void*;
	static auto Release(void* ptr) -> void;

};

