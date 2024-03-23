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
	Error BindAny(uint16 port)												override;
	Error Bind(std::string addr, uint16 port)								override;
	Error Accept()															override;
	UN_USED Error Connect(DWORD* bytes)										override;
	Error Recv(WSABUF* buf, DWORD* bytes)									override;
	UN_USED Error Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	auto Accept(AsyncEndpoint* client)						-> Error;
	auto Send(AsyncEndpoint* dest, CHAR* msg, size_t size)	-> Error;

public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKET&			GetSocketRef()			override;
	SOCKADDR_IN&	GetAddrRef()			override;

public:
	auto SetSocket(SOCKET socket)			-> void;
	auto SetTransferredBytes(DWORD bytes)	-> void;

public:
	auto GetAsyncStreamRef()						-> AsyncEndpoint&;
	auto SocketAcceptUpdate(AsyncEndpoint* client)	-> Error;
	auto GetTransferredBytesRef()					-> DWORD&;

private:
	AsyncEndpoint mListener;
};

