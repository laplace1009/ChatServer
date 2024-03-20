#pragma once
#include "Network.h"
#include "Memory.h"

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
	AsyncStream*	owner;
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
	static auto Init() -> bool;

public:
	bool BindAny(uint16 port)					override;
	bool Bind(std::string addr, uint16 port)	override;
	bool Connect()								override;
	bool Recv()									override;
	bool Send(CHAR* msg, size_t size)			override;

public:
	const	SOCKET ConstGetSocket() const			override;
	SOCKET& GetSocketRef()							override;
	SOCKADDR_IN& GetAddrRef()						override;
	WSABUF& GetRecvBufRef()							override;
	WSABUF& GetSendBufRef()							override;
	const DWORD GetRecvBytes() const				override;
	const DWORD GetSendBytes() const				override;

public:
	auto GetOverlappedRef()						-> LPOVERLAPPEDEX&;
	auto GetLPOverlappedPtr()					-> OverlappedEx**;
	auto GetIOEvent()							-> IOEvent;
	auto SetIOEvent(IOEvent event)				-> void;
	auto SetAddr(std::string addr, uint16 port) -> bool;
	auto GetSendBytesRef()						-> DWORD&;
	auto SetRecvBytes(DWORD bytes)				-> void;
	auto GetRecvBytesRef()						-> DWORD&;
	auto SetSendBytes(DWORD bytes)				-> void;
	auto SocketConnectUpdate()					-> bool;
	auto SocketReuseAddr()						-> bool;
	auto SocketTcpNoDelay()						-> bool;

private:
	auto setMsg(WSABUF& dest, CHAR* msg, size_t size) -> bool;

private:
	static auto bindWsaIoctl(GUID guid, LPVOID* fn) -> bool;

private:
	LPOVERLAPPEDEX	mOverlapped;
	SOCKET			mSocket;
	SOCKADDR_IN		mAddr;
	WSABUF			mRecvBuf;
	WSABUF			mSendBuf;
	DWORD			mRecvBytes;
	DWORD			mSendBytes;
};

