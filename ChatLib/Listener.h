#pragma once
#include "Stream.h"

class Listener
{
public:
	virtual ~Listener() {}

public:									
	virtual bool BindAny(uint16 port)								= 0;
	virtual bool Bind(std::string addr, uint16 port)				= 0;
	virtual bool Accept()											= 0;
	virtual bool Recv()												= 0;
	virtual bool Send(std::wstring msg, DWORD msgLen)				= 0;
	virtual bool SetSendMessage(std::wstring msg, DWORD msgSize)	= 0;

public:
	virtual const SOCKET ConstGetSocket() const	= 0;
	virtual void SetSocket(SOCKET socket)		= 0;
	virtual SOCKADDR_IN& GetAddrRef()			= 0;
	virtual WSABUF& GetRecvBufRef()				= 0;
	virtual const DWORD GetRecvBytes() const	= 0;
	//virtual void SetRecvBytes(DWORD bytes)		= 0;
	virtual WSABUF& GetSendBufRef()				= 0;
	virtual const DWORD GetSendBytes() const	= 0;
	//virtual void SetSendBytes(DWORD bytes)		= 0;
};

