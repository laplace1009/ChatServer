#pragma once
#include "Types.h"
#include <memory>

class BaseAllocator
{

public:
	static auto Allocate(uint64 size) -> void*;
	static auto Release(void* ptr) -> void;
};
