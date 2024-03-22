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
	return ::accept(mListener.GetEndpointRef().ConstGetSocket(), reinterpret_cast<PSOCKADDR>(&client->GetAddrRef()), &addrLen);
}

bool TcpListener::Recv(WSABUF* buf, DWORD* bytes)
{
	return mListener.Recv();
}

bool TcpListener::Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)
{
	return mListener.Send(buf);
}

auto TcpListener::Accept(TcpEndpoint* client) -> bool
{
	int addrLen = sizeof(SOCKADDR_IN);
	return ::accept(mListener.GetEndpointRef().ConstGetSocket(), reinterpret_cast<PSOCKADDR>(&client->GetEndpointRef().GetAddrRef()), &addrLen);
}

auto TcpListener::Send(TcpEndpoint* dest, CHAR* msg, size_t size) -> bool
{
	return ::send(dest->GetEndpointRef().ConstGetSocket(), msg, static_cast<int>(size), 0) != SOCKET_ERROR;
}


const SOCKET TcpListener::ConstGetSocket() const
{
	return mListener.ConstGetEndPointRef().ConstGetSocket();
}

SOCKADDR_IN& TcpListener::GetAddrRef()
{
	return mListener.GetEndpointRef().GetAddrRef();
}

auto TcpListener::SetSocket(SOCKET socket) -> void
{
	mListener.GetEndpointRef().GetSocketRef() = socket;
}

auto TcpListener::SetTransferredBytes(DWORD bytes) -> void
{
	mListener.GetTransferredBytesRef() = bytes;
}

