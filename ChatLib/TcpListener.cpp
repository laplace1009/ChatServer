#include "pch.h"
#include "TcpListener.h"

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

// ±¸Çö
bool TcpListener::Recv()
{
	return mListener.Recv();
}

bool TcpListener::Send(CHAR* msg, size_t size)
{
	return ::send(mListener.ConstGetSocket(), msg, static_cast<int>(size), 0);
}

auto TcpListener::Accept(TcpStream* client) -> bool
{
	int addrLen = sizeof(SOCKADDR_IN);
	return ::accept(mListener.ConstGetSocket(), reinterpret_cast<PSOCKADDR>(&client->GetAddrRef()), &addrLen);
}

auto TcpListener::Send(TcpStream* dest, CHAR* msg, size_t size) -> bool
{
	return ::send(dest->ConstGetSocket(), msg, static_cast<int>(size), 0) != SOCKET_ERROR;
}

const SOCKET TcpListener::ConstGetSocket() const
{
	return mListener.ConstGetSocket();
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
	return mListener.GetRecvBytes();
}

WSABUF& TcpListener::GetSendBufRef()
{
	return mListener.GetSendBufRef();
}

const DWORD TcpListener::GetSendBytes() const
{
	return mListener.GetSendBytes();
}

auto TcpListener::SetSocket(SOCKET socket) -> void
{
	mListener.GetSocketRef() = socket;
}

auto TcpListener::SetRecvBytes(DWORD bytes) -> void
{
	mListener.SetRecvBytes(bytes);
}

auto TcpListener::SetSendBytes(DWORD bytes) -> void
{
	mListener.SetSendBytes(bytes);
}

