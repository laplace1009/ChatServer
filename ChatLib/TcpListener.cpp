#include "pch.h"
#include "TcpListener.h"
#include "OverlappedEx.h"

bool TcpListener::BindAny(uint16 port)
{
	return mListener.BindAny(port);
}

bool TcpListener::Bind(std::string addr, uint16 port)
{
	return mListener.Bind(addr, port);
}

bool TcpListener::Accept()
{
	TcpStream* client = xnew<TcpStream>();
	int addrLen = sizeof(SOCKADDR_IN);
	return ::accept(mListener.ConstGetSocket(), reinterpret_cast<PSOCKADDR>(&client->GetAddrRef()), &addrLen);
}

bool TcpListener::Recv()
{
	return mListener.Recv();
}

bool TcpListener::Send(std::wstring msg, DWORD msgLen)
{
	if (SetSendMessage(msg, msgLen) == false)
		return false;

	return mListener.Send();
}

bool TcpListener::SetSendMessage(std::wstring msg, DWORD msgSize)
{
	return memcpy_s(mListener.GetSendBufRef().buf, mListener.GetSendBufRef().len, msg.c_str(), msg.size() * sizeof(wchar_t)) == 0;
}

const SOCKET TcpListener::ConstGetSocket() const
{
	return mListener.ConstGetSocket();
}

void TcpListener::SetSocket(SOCKET socket)
{
	mListener.SetSocket(socket);
}

SOCKADDR_IN& TcpListener::GetAddrRef()
{
	return mListener.GetAddrRef();
}

WSABUF& TcpListener::GetRecvBufRef()
{
	return mListener.GetRecvBufRef();
}

const DWORD TcpListener::GetRecvBytes() const
{
	return mListener.ConstGetRecvBytes();
}

//void TcpListener::SetRecvBytes(DWORD bytes)
//{
//	mStream.get
//}

WSABUF& TcpListener::GetSendBufRef()
{
	return mListener.GetSendBufRef();
}

const DWORD TcpListener::GetSendBytes() const
{
	return mListener.ConstGetSendBytes();
}

//void TcpListener::SetSendBytes(DWORD bytes)
//{
//}

