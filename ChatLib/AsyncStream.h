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
	~AsyncStream() noexcept;

public:
	static auto Init()			->	bool;
	static auto CreateSocket()	->	bool;

public:
	bool Bind()			override;
	bool Connect()		override;
	bool Recv()			override;
	bool Send()			override;

public:
	auto GetSocket()		->	SOCKET;
	auto GetOverlappedPtr() ->	OverlappedEx*;

private:
	static auto bindWsaIoctl(GUID guid, LPVOID* fn) -> bool;

private:
	OverlappedEx*	mOverlapped;
	SOCKET			mSocket;
	SOCKADDR_IN		mAddr;
	WSABUF			mBuf;
	DWORD			mRecvBytes;
	DWORD			mSendBytes;
};

