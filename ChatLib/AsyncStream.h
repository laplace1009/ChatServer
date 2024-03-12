#pragma once
#include "Stream.h"
#include "TcpStream.h"
#include "OverlappedEx.h"
#include "Memory.h"

class AsyncStream : public Stream
{
public:
	static LPFN_CONNECTEX				ConnectEx;
	static LPFN_DISCONNECTEX			DisconnectEx;
	static LPFN_ACCEPTEX				AcceptEx;
	static LPFN_GETACCEPTEXSOCKADDRS	GetAcceptExSockaddrs;

public:
	AsyncStream();
	virtual ~AsyncStream() noexcept;

public:
	static auto Init()			->	bool;
	static auto CreateSocket()	->	int;

public:
	bool Bind()		override;
	bool Connect()	override;
	bool Recv()		override;
	bool Send()		override;

public:
	const	SOCKET ConstGetSocket() const			override;
	void	SetSocket(SOCKET socket)				override;
	const	SOCKADDR_IN& ConstGetAddrRef()			override;
	bool	SetAddr(std::string addr, uint16 port)	override;
	const	WSABUF& ConstGetRecvBufRef()			override;
	WSABUF& GetRecvBufRef()							override;
	const	DWORD ConstGetRecvBytes()				override;
	void	SetRecvBytes(DWORD bytes)				override;
	const	DWORD ConstGetSendBytes()				override;
	void	SetSendBytes(DWORD bytes)				override;

public:
	auto GetOverlappedPtr() ->	OverlappedEx*;

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

