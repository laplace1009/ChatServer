#pragma once
#include "Types.h"

// ������ �����ͷ� �Ű������� ����� �ٿ��ִ� �±�
#define OUT

#define XALLOCATE(size)	PoolAllocator::Allocate(size)
#define XRELEASE(ptr)	PoolAllocator::Release(ptr)

// �ǵ������� ũ������ ���� ���� ��ũ��1
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}											

// �ǵ������� ũ������ ���� ���� ��ũ��2
#define ASSERT_CRASH(expr)					\
{											\
	if ((expr == false))					\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}