#pragma once
#include "Listener.h"
#include "AsyncStream.h"

class AsyncListener : public Listener
{
public:
	AsyncListener();
	~AsyncListener() noexcept override = default;

public:
	bool BindAny(uint16 port)					override;
	bool Bind(std::string addr, uint16 port)	override;
	bool Accept()								override;
	bool Recv()									override;
	bool Send(CHAR* msg, size_t size)			override;

public:
	auto Accept(AsyncStream* client)						-> bool;
	auto Send(AsyncStream* dest, CHAR* msg, size_t size)	-> bool;
public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKET&			GetSocketRef()			override;
	SOCKADDR_IN&	GetAddrRef()			override;
	WSABUF&			GetRecvBufRef()			override;
	WSABUF&			GetSendBufRef()			override;
	const DWORD		GetRecvBytes() const	override;
	const DWORD		GetSendBytes() const	override;

public:
	auto SetSocket(SOCKET socket) -> void;
	auto SetRecvBytes(DWORD bytes) -> void;
	auto SetSendBytes(DWORD bytes) -> void;

public:
	auto GetAsyncStreamRef()						-> AsyncStream&;
	auto SocketAcceptUpdate(AsyncStream* client)	-> bool;
	auto GetSendBytesRef()							-> DWORD&;
	auto GetRecvBytesRef()							-> DWORD&;

private:
	AsyncStream mListener;
};

