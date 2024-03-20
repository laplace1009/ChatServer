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
	~TcpStream() noexcept override;
	
public:
	bool BindAny(uint16 port)							override;
	bool Bind(std::string addr, uint16 port)			override;
	bool Connect()										override;
	bool Recv()											override;
	bool Send(CHAR* msg, size_t size)					override;

public:
	const SOCKET	ConstGetSocket() const	override;
	SOCKET&			GetSocketRef()			override;
	SOCKADDR_IN&	GetAddrRef()			override;
	WSABUF&			GetRecvBufRef()			override;
	WSABUF&			GetSendBufRef()			override;
	const DWORD		GetRecvBytes() const	override;
	const DWORD		GetSendBytes() const	override;

public:
	auto SetAddr(std::string addr, uint16 port) -> void;
	auto SetRecvBytes(DWORD bytes)				-> void;
	auto SetSendBytes(DWORD bytes)				-> void;
	
private:
	SOCKET		mSocket;
	SOCKADDR_IN mAddr;
	WSABUF		mRecvBuf;
	WSABUF		mSendBuf;
	DWORD		mRecvBytes;
	DWORD		mSendBytes;
};

