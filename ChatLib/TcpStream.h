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
	Error BindAny(uint16 port)										override;
	Error Bind(std::string addr, uint16 port)						override;
	Error Connect(DWORD* bytes)										override;
	Error Recv(WSABUF* buf, DWORD* bytes)							override;
	Error Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	override;

public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKET&			GetSocketRef()			override;
	SOCKADDR_IN&	GetAddrRef()			override;

public:
	auto SetAddr(std::string addr, uint16 port) -> void;
	auto SocketConnectUpdate() -> Error;
	auto SocketReuseAddr() -> Error;
	auto SocketTcpNoDelay() -> Error;
	
private:
	SOCKET		mSocket;
	SOCKADDR_IN mAddr;
};

