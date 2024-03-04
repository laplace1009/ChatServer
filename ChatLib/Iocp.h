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
	auto GetHandlePtr() -> HANDLE*;
	auto Register(TcpStream& stream) -> bool;
	//auto Dispatch(TcpStream& stream, uint32 timeout = INFINITE) -> bool;

private:
	HANDLE mHandle;
};

