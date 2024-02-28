#pragma once
#include "Types.h"

// 참조나 포인터로 매개변수값 변경시 붙여주는 태그
#define OUT

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