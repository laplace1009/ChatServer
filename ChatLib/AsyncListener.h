#pragma once
#include "Listener.h"
#include "NetworkEndpoint.h"
#include "AsyncStream.h"

class AsyncListener : public Listener
{

public:
	AsyncListener();
	~AsyncListener() noexcept override = default;

public:
	bool BindAny(uint16 port)												override;
	bool Bind(std::string addr, uint16 port)								override;
	bool Accept()															override;
	UN_USED bool Connect(DWORD* bytes)										override;
	bool Recv(WSABUF* buf, DWORD* bytes)									override;
	UN_USED bool Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	auto Accept(AsyncEndpoint* client)						-> bool;
	auto Send(AsyncEndpoint* dest, CHAR* msg, size_t size)	-> bool;

public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKET&			GetSocketRef()			override;
	SOCKADDR_IN&	GetAddrRef()			override;

public:
	auto SetSocket(SOCKET socket)			-> void;
	auto SetTransferredBytes(DWORD bytes)	-> void;

public:
	auto GetAsyncStreamRef()						-> AsyncEndpoint&;
	auto SocketAcceptUpdate(AsyncEndpoint* client)	-> bool;
	auto GetTransferredBytesRef()					-> DWORD&;

private:
	AsyncEndpoint mListener;
};

