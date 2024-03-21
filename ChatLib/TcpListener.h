#pragma once
#include "TcpStream.h"
#include "Listener.h"
#include "NetworkEndpoint.h"

class TcpListener : public Listener
{
public:
	bool BindAny(uint16 port)										override;
	bool Bind(std::string addr, uint16 port)						override;
	bool Accept()													override;
	bool Recv(WSABUF* buf, DWORD* bytes)							override;
	bool Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	auto Accept(TcpEndpoint* client) -> bool;
	auto Send(TcpEndpoint* dest, CHAR* msg, size_t size) -> bool;

public:
	const SOCKET ConstGetSocket() const	override;
	SOCKADDR_IN& GetAddrRef()			override;

public:
	auto SetSocket(SOCKET socket)	-> void;
	auto SetTransferredBytes(DWORD bytes) -> void;

private:
	TcpEndpoint mListener;
};

