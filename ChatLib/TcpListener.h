#pragma once
#include <string_view>
#include "TcpStream.h"
#include "Listener.h"

class TcpListener : public Listener
{
public:
	bool BindAny(uint16 port)					override;
	bool Bind(std::string addr, uint16 port)	override;
	bool Accept()								override;
	bool Recv()									override;
	bool Send(CHAR* msg, size_t size)			override;

public:
	auto Accept(TcpStream* clien) -> bool;
	auto Send(TcpStream* dest, CHAR* msg, size_t size) -> bool;

public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKADDR_IN&	GetAddrRef()			override;
	WSABUF&			GetRecvBufRef()			override;
	const DWORD		GetRecvBytes() const	override;
	WSABUF&			GetSendBufRef()			override;
	const DWORD		GetSendBytes() const	override;

public:
	auto SetSocket(SOCKET socket)	-> void;
	auto SetRecvBytes(DWORD bytes)	-> void;
	auto SetSendBytes(DWORD bytes)	-> void;

private:
	TcpStream mListener;
};

