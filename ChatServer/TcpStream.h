#pragma once
#include "Types.h"
#include <string_view>
#include <optional>
#include <variant>
#include <WinSock2.h>


class TcpStream
{
	enum
	{
		MAX_BUFF_SIZE = 4096,
	};

	struct SocketInfo
	{
		OVERLAPPED overlapped;
		SOCKET socket;
		SOCKADDR_IN addr;
		CHAR* buf;
		WSABUF wsaBuf;
		DWORD recvBytes;
		DWORD sendBytes;
	};

public:
	TcpStream();
	~TcpStream() noexcept;

public:
	auto Connect(std::string_view addr, uint16 port) -> int;
	auto Recv(uint32 offset) -> int;
	auto Send(CHAR* message, uint32 msgLength, uint32 offset, DWORD bufCount) -> int;

public:
	auto SetSocketOpt(int option) -> int;
	auto GetSocketInfoPtr() -> SocketInfo*;
	auto GetMaxBuffSize() -> uint32;

private:
	SocketInfo mSocket;
};

