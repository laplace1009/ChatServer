#pragma once
#include "Types.h"
#include "TcpStream.h"
#include "TcpListener.h"

class Iocp
{
public:
	~Iocp() noexcept;

public:
	auto Init() -> bool;
	auto GetHandle() -> HANDLE;
	auto SetHandle(HANDLE h) -> void;
	auto Register(TcpStream& stream) -> bool;
	//auto Dispatch(TcpStream& stream, uint32 timeout = INFINITE) -> bool;

private:
	HANDLE mHandle;
};

