#pragma once
#include "Types.h"

// 참조나 포인터로 매개변수값 변경시 붙여주는 태그
#define OUT

// 기본적인 [[nodiscard]] 매크로
#define NODISCARD [[nodiscard]]

// 메시지를 포함하는 [[nodiscard]] 매크로
#if __cplusplus >= 202002L  // C++20 이상인 경우에만 메시지 포함 버전 사용 가능
#define NODISCARD_MSG(msg) [[nodiscard(msg)]]
#else
#define NODISCARD_MSG(msg) [[nodiscard]]
#endif


#define XALLOCATE(size)	PoolAllocator::Allocate(size)
#define XRELEASE(ptr)	PoolAllocator::Release(ptr)

// 의도적으로 크래쉬를 내기 위한 매크로1
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}											

// 의도적으로 크래쉬를 내기 위한 매크로2
#define ASSERT_CRASH(expr)					\
{											\
	if ((expr == false))					\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}