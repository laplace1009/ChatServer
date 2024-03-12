#pragma once
#include <string_view>
#include "TcpStream.h"
#include "Listener.h"

class TcpListener : public Listener
{
public:
	bool BindAny(uint16 port)								override;
	bool Bind(std::string addr, uint16 port)				override;
	bool Accept()											override;
	bool Recv()												override;
	bool Send(std::wstring msg, DWORD msgLen)				override;
	bool SetSendMessage(std::wstring msg, DWORD msgSize)	override;

public:
	const SOCKET	ConstGetSocket() const		override;
	void			SetSocket(SOCKET socket)	override;
	SOCKADDR_IN&	GetAddrRef()				override;
	WSABUF&			GetRecvBufRef()				override;
	const DWORD		GetRecvBytes() const		override;
	//void			SetRecvBytes(DWORD bytes)	override;
	WSABUF&			GetSendBufRef()				override;
	const DWORD		GetSendBytes() const		override;
	//void			SetSendBytes(DWORD bytes)	override;

private:
	TcpStream mListener;
};

