#pragma once
#include "Stream.h"
#include "OverlappedEx.h"
#include "Memory.h"

class OverlappedEx;
enum class IOEvent;

class AsyncStream : public Stream
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
	static auto Init()			->	bool;
	static auto CreateSocket()	->	SOCKET;

public:
	bool BindAny(uint16 port)					override;
	bool Bind(std::string addr, uint16 port)	override;
	bool Connect()								override;
	bool Recv()									override;
	bool Send(CHAR* msg, size_t size)			override;

public:
	const	SOCKET ConstGetSocket() const			override;
	void	SetSocket(SOCKET socket)				override;
	const	SOCKADDR_IN& ConstGetAddrRef() const	override;
	SOCKADDR_IN& GetAddrRef()						override;
	bool	SetAddr(std::string addr, uint16 port)	override;
	const	WSABUF& ConstGetRecvBufRef() const		override;
	WSABUF& GetRecvBufRef()							override;
	const	DWORD ConstGetRecvBytes() const			override;
	void	SetRecvBytes(DWORD bytes)				override;
	const	WSABUF& ConstGetSendBufRef() const		override;
	WSABUF& GetSendBufRef()							override;
	const	DWORD ConstGetSendBytes() const			override;
	void	SetSendBytes(DWORD bytes)				override;

public:
	auto GetSocketPtr() -> SOCKET*;
	auto GetOverlappedPtr() -> OverlappedEx*;
	auto GetLPOverlappedPtr() -> OverlappedEx**;
	auto GetIOEvent() -> IOEvent;
	auto GetSendBytesRef() -> DWORD&;
	auto GetRecvBytesRef() -> DWORD&;
	auto SocketConnectUpdate() -> bool;
	auto SocketReuseAddr() -> bool;

private:
	auto setMsg(WSABUF& dest, CHAR* msg, size_t size) -> bool;

private:
	static auto bindWsaIoctl(GUID guid, LPVOID* fn) -> bool;

private:
	OverlappedEx*	mOverlapped;
	SOCKET			mSocket;
	SOCKADDR_IN		mAddr;
	WSABUF			mRecvBuf;
	WSABUF			mSendBuf;
	DWORD			mRecvBytes;
	DWORD			mSendBytes;
};

