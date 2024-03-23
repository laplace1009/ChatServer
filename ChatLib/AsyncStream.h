#pragma once
#include "Network.h"
#include "Memory.h"
#include "Error.h"

class AsyncStream;

enum class IOEvent
{
	CONNECT,
	ACCEPT,
	RECV,
	SEND,
	DISCONNECT,
};

typedef struct OverlappedEx : WSAOVERLAPPED
{
	IOEvent			ioEvent;
	void*			owner;
} OVERLAPPEDEX, * LPOVERLAPPEDEX;

class alignas(16) AsyncStream : public Network
{
public:
	static LPFN_CONNECTEX				ConnectEx;
	static LPFN_DISCONNECTEX			DisconnectEx;
	static LPFN_ACCEPTEX				AcceptEx;
	static LPFN_GETACCEPTEXSOCKADDRS	GetAcceptExSockaddrs;

public:
	AsyncStream();
	~AsyncStream() noexcept override;

public:
	static auto Init() -> Error;

public:
	Error BindAny(uint16 port)										override;
	Error Bind(std::string addr, uint16 port)						override;
	Error Connect(DWORD* bytes)										override;
	Error Recv(WSABUF* buf, DWORD* bytes)							override;
	Error Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	const	SOCKET ConstGetSocket() const	override;
	SOCKET& GetSocketRef()					override;
	SOCKADDR_IN& GetAddrRef()				override;

public:
	auto GetOverlappedRef()						-> LPOVERLAPPEDEX&;
	auto GetLPOverlappedPtr()					-> OverlappedEx**;
	auto GetIOEvent()							-> IOEvent;
	auto SetIOEvent(IOEvent event)				-> void;
	auto SetAddr(std::string addr, uint16 port) -> bool;
	auto SocketConnectUpdate()					-> Error;
	auto SocketReuseAddr()						-> Error;
	auto SocketTcpNoDelay()						-> Error;

private:
	auto setMsg(WSABUF& dest, CHAR* msg, size_t size) -> bool;

private:
	static auto bindWsaIoctl(GUID guid, LPVOID* fn) -> bool;

private:
	LPOVERLAPPEDEX	mOverlapped;
	SOCKET			mSocket;
	SOCKADDR_IN		mAddr;
};

