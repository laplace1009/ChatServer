#pragma once
#include "Types.h"
#include <string_view>
#include <optional>
#include <variant>
#include <WinSock2.h>

enum class IOEvent
{
	ACCEPT,
	RECV,
	SEND,
	DISCONNECT,
};

struct OVERLAPPEDEX
{
	WSAOVERLAPPED wSaOverlapped;
	IOEvent event;
};


class TcpStream
{
	enum
	{
		MAX_BUFF_SIZE = 4096,
	};

	struct SocketInfo
	{
		OVERLAPPEDEX overlapped;
		SOCKET socket;
		SOCKADDR_IN addr;
		CHAR* buf;
		WSABUF wsaBuf;
		DWORD recvBytes;
		DWORD sendBytes;
	};

public:
	static LPFN_CONNECTEX		LpFnConnectEx;
	static LPFN_DISCONNECTEX	LpFnDisconnectEx;
	static LPFN_ACCEPTEX		LpFnAcceptEx;

public:
	auto Init() -> bool;
	auto Close() -> void;
	auto Connect(std::string_view addr, uint16 port) -> int;
	auto Recv(uint32 offset) -> int;
	auto Send(CHAR* message, uint32 msgLength, uint32 offset, DWORD bufCount) -> int;

public:
	auto SetSocketOpt(int option) -> int;
	auto GetSocketInfoPtr() -> SocketInfo*;
	auto GetSocketInfoPtr() const -> const SocketInfo*;
	auto GetMaxBuffSize() -> uint32;

private:
	auto bindWsaIoctl(GUID guid, LPVOID* fn) -> bool;

private:
	SocketInfo mSocket;
};

