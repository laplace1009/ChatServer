#pragma once
#include "Types.h"
#include "MemoryHeader.h"
#include "MemoryPool.h"

// 같은 object끼리 같은 메모리를 사용하는것
template<typename Type>
class ObjectPool
{
public:
	template<typename... Args>
	static auto Pop(Args&&... args) -> Type*
	{
		Type* memory = static_cast<Type*>(MemoryHeader::AttachHeader(mPool.Pop(), mAllocSize));
		new(memory)Type(std::forward<Args>(args)...);
		return memory;
	}

	static auto Push(Type* obj) -> void
	{
		obj->~Type();
		mPool.Push(MemoryHeader::DetachHeader(obj));
	}

private:
	static uint32 mAllocSize;
	static MemoryPool mPool;
};

template<typename Type>
uint32 ObjectPool<Type>::mAllocSize = sizeof(Type) + sizeof(MemoryHeader);

template<typename Type>
MemoryPool ObjectPool<Type>::mPool{ mAllocSize };

