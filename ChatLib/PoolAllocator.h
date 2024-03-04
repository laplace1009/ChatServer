#pragma once
class PoolAllocator
{
public:
	static auto Allocate(int32 size) -> void*;
	static auto Release(void* ptr) -> void;
};

