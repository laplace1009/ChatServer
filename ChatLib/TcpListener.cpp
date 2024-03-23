#include "pch.h"
#include "TcpListener.h"

Error TcpListener::BindAny(uint16 port)
{
	return mListener.BindAny(port);
}

Error TcpListener::Bind(std::string addr, uint16 port)
{
	return mListener.Bind(addr, port);
}

Error TcpListener::Accept()
{
	TcpStream* client = xnew<TcpStream>();
	int addrLen = sizeof(SOCKADDR_IN);
	return ::accept(mListener.GetEndpointRef().ConstGetSocket(), reinterpret_cast<PSOCKADDR>(&client->GetAddrRef()), &addrLen) != INVALID_SOCKET ? Error::OK : Error::NET_ACCEPT_ERROR;
}

Error TcpListener::Recv(WSABUF* buf, DWORD* bytes)
{
	return mListener.Recv();
}

Error TcpListener::Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)
{
	return mListener.Send(buf);
}

auto TcpListener::Accept(TcpEndpoint* client) -> Error
{
	int addrLen = sizeof(SOCKADDR_IN);
	return INVALID_SOCKET != ::accept(mListener.GetEndpointRef().ConstGetSocket(), reinterpret_cast<PSOCKADDR>(&client->GetEndpointRef().GetAddrRef()), &addrLen) ? Error::OK : Error::NET_ACCEPT_ERROR;
}

auto TcpListener::Send(TcpEndpoint* dest, CHAR* msg, size_t size) -> Error
{
	return SOCKET_ERROR != ::send(dest->GetEndpointRef().ConstGetSocket(), msg, static_cast<int>(size), 0) ? Error::OK : Error::NET_SEND_ERROR;
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