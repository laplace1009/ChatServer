#pragma once
#include "Types.h"
#include "Stream.h"
#include <string>
#include <WinSock2.h>

class alignas(16) TcpStream: public Stream
{
public:
	TcpStream();
	virtual ~TcpStream() noexcept;

public:
	static auto CreateSocket() -> int;
	
public:
	bool Bind()		override;
	bool Connect()	override;
	bool Recv()		override;
	bool Send()		override;

public:
	const	SOCKET ConstGetSocket() const			override;
	void	SetSocket(SOCKET socket)				override;
	const	SOCKADDR_IN& ConstGetAddrRef()			override;
	bool	SetAddr(std::string addr, uint16 port)	override;
	const	WSABUF& ConstGetRecvBufRef()			override;
	WSABUF& GetRecvBufRef()							override;
	const	DWORD ConstGetRecvBytes()				override;
	void	SetRecvBytes(DWORD size)				override;
	const	DWORD ConstGetSendBytes()				override;
	void	SetSendBytes(DWORD bytes)				override;

private:
	SOCKET		mSocket;
	SOCKADDR_IN mAddr;
	WSABUF		mRecvBuf;
	WSABUF		mSendBuf;
	DWORD		mRecvBytes;
	DWORD		mSendBytes;
};

