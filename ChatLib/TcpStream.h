#pragma once
#include "Types.h"
#include <string>
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
		MAX_BUFF_SIZE = 2048,
	};

	struct alignas(16) SocketInfo
	{
		OVERLAPPEDEX overlapped;
		SOCKET socket;
		SOCKADDR_IN addr;
		WSABUF buf;
		DWORD recvBytes;
		DWORD sendBytes;
	};

public:
	static LPFN_CONNECTEX		LpFnConnectEx;
	static LPFN_DISCONNECTEX	LpFnDisconnectEx;
	static LPFN_ACCEPTEX		LpFnAcceptEx;

public:
	static auto Send(TcpStream& stream) -> bool;
	static auto Recv(TcpStream& stream) -> bool;

public:
	auto Init() -> bool;
	auto Close() -> void;
	
	auto Bind() -> bool;
	auto Connect() -> bool;
	auto Connect(std::string addr, uint16 port) -> bool;

public:
	auto GetSocket() const -> const SOCKET;
	auto GetSocket() -> SOCKET;
	auto SetSocket(SOCKET socket) -> void;
	auto GetAddrPtr() -> SOCKADDR_IN*;
	auto SetAddr(std::string addr, uint16 port) -> bool;
	auto GetBuffer() -> WSABUF*;
	auto GetRecvBytes() const -> const DWORD;
	auto SetRecvBtyes(DWORD size) -> void;
	auto GetSendBytes() const -> const DWORD;
	auto GetSendBytes() -> DWORD;
	auto SetSendBytes(DWORD size) -> void;
	auto GetOverlappedPtr() -> LPOVERLAPPED;
	auto GetIOEvent() const -> const IOEvent;
	auto GetIOEvent() -> IOEvent;
	auto SetIOEvent(IOEvent event) -> void;
	auto SetSocketOpt(int option) -> bool;
	auto GetSocketInfoPtr() -> SocketInfo*;
	auto GetSocketInfoPtr() const -> const SocketInfo*;
	auto GetMaxBuffSize() -> uint32;

private:
	auto bindWsaIoctl(GUID guid, LPVOID* fn) -> bool;

private:
	SocketInfo mSocket;
};

