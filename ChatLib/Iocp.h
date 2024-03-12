#pragma once
#include "Types.h"
#include "AsyncStream.h"

class Iocp
{
public:
	~Iocp() noexcept;

public:
	auto Init()									-> bool;
	auto GetHandle()							-> HANDLE;
	auto SetHandle(HANDLE h)					-> void;
	auto Register(AsyncStream& stream)			-> bool;
	auto Dispatch(uint32 timeout = INFINITE)	-> bool;

private:
	HANDLE mHandle;
};

