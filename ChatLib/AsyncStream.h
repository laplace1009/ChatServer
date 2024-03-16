#pragma once
#include "Stream.h"
#include "TcpStream.h"
#include "OverlappedEx.h"
#include "Memory.h"

class OverlappedEx;

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
	bool BindAny(uint16 port)						override;
	bool Bind(std::string addr, uint16 port)		override;
	bool Connect()									override;
	bool Recv()										override;
	bool Send()										override;

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
	auto GetOverlappedPtr() -> OverlappedEx*;
	auto GetLPOverlappedPtr() -> OverlappedEx**;
	auto GetIOEvent() -> IOEvent;

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

