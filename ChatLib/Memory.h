#pragma once
#include "PoolAllocator.h"

template<typename Type, typename... Args>
auto xnew(Args&&... args) -> Type*
{
	Type* memory = reinterpret_cast<Type*>(PoolAllocator::Allocate(sizeof(Type)));

	// placement new
	new(memory)Type(std::forward<Args>(args)...);

	return memory;
}

template<typename Type>
auto xdelete(Type* obj) -> void
{
	obj->~Type();
	PoolAllocator::Release(obj);
}

