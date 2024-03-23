#pragma once
#include "TcpStream.h"
#include "Error.h"
#include "Listener.h"
#include "NetworkEndpoint.h"

class TcpListener : public Listener
{
public:
	Error BindAny(uint16 port)										override;
	Error Bind(std::string addr, uint16 port)						override;
	Error Accept()													override;
	Error Recv(WSABUF* buf, DWORD* bytes)							override;
	Error Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	auto Accept(TcpEndpoint* client) -> Error;
	auto Send(TcpEndpoint* dest, CHAR* msg, size_t size) -> Error;

public:
	const SOCKET ConstGetSocket() const	override;
	SOCKADDR_IN& GetAddrRef()			override;

public:
	auto SetSocket(SOCKET socket)	-> void;
	auto SetTransferredBytes(DWORD bytes) -> void;

private:
	TcpEndpoint mListener;
};