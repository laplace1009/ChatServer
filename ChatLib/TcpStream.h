#pragma once
#include "Types.h"
#include "Network.h"
#include "Memory.h"
#include <string>
#include <WinSock2.h>

class alignas(16) TcpStream: public Network
{
public:
	TcpStream();
	~TcpStream() noexcept override = default;
	
public:
	bool BindAny(uint16 port)										override;
	bool Bind(std::string addr, uint16 port)						override;
	bool Connect(DWORD* bytes)										override;
	bool Recv(WSABUF* buf, DWORD* bytes)							override;
	bool Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKET&			GetSocketRef()			override;
	SOCKADDR_IN&	GetAddrRef()			override;

public:
	auto SetAddr(std::string addr, uint16 port) -> void;
	auto SocketConnectUpdate() -> bool;
	auto SocketReuseAddr() -> bool;
	auto SocketTcpNoDelay() -> bool;
	
private:
	SOCKET		mSocket;
	SOCKADDR_IN mAddr;
};

