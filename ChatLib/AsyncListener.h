#pragma once
#include "Types.h"
#include "AsyncStream.h"
#include "Listener.h"
#include "Iocp.h"

class AsyncListener : public Listener
{
public:
	AsyncListener();
	virtual ~AsyncListener();

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

public:
	auto GetAsyncStreamRef() -> AsyncStream&;

public:
	auto AcceptRegister(AsyncStream* cilent) -> void;

private:
	AsyncStream mListener;
	std::vector<AsyncStream*> mClients;
};

