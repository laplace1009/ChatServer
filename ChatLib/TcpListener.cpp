#include "pch.h"
#include "TcpListener.h"

auto TcpListener::Init() -> bool
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return false;

	return mStream.Init();
}

auto TcpListener::BindAny(uint16 port) -> void
{
	mStream.GetAddrPtr()->sin_family = AF_INET;
	mStream.GetAddrPtr()->sin_addr.s_addr = htonl(INADDR_ANY);
	mStream.GetAddrPtr()->sin_port = htons(port);
	if (SOCKET_ERROR == bind(mStream.GetSocket(), reinterpret_cast<SOCKADDR*>(mStream.GetAddrPtr()), sizeof(SOCKADDR_IN)))
		return;

	if (SOCKET_ERROR == listen(mStream.GetSocket(), SOMAXCONN))
		return;
}

auto TcpListener::Bind(std::string addr, uint16 port) -> void
{
	mStream.GetAddrPtr()->sin_family = AF_INET;
	mStream.GetAddrPtr()->sin_port = htons(port);
	int error = inet_pton(AF_INET, addr.c_str(), mStream.GetAddrPtr());
	// wrong string address
	if (0 == error)
	{
		return;
	}
	// internal error
	if (-1 == error)
	{
		return;
	}
	if (SOCKET_ERROR == bind(mStream.GetSocket(), reinterpret_cast<SOCKADDR*>(mStream.GetAddrPtr()), sizeof(SOCKADDR_IN)))
		return;
	if (SOCKET_ERROR == listen(mStream.GetSocket(), SOMAXCONN))
		return;
}

auto TcpListener::Accept() -> TcpStream
{
	TcpStream client;
	int addrLen = sizeof(client.GetSocketInfoPtr()->addr);
	client.SetSocket(accept(mStream.GetSocket(), reinterpret_cast<SOCKADDR*>(client.GetAddrPtr()), &addrLen));

	return client;
}

auto TcpListener::Recv(OUT TcpStream& client) -> int
{
	return recv(client.GetSocket(), reinterpret_cast<char*>(client.GetBuffer()->buf), sizeof(SOCKADDR_IN), 0);
}

auto TcpListener::Send(TcpStream& client) -> int
{
	return mStream.Send(client);
}

auto TcpListener::SwitchSyncAsync(u_long swt) -> int
{
	return ioctlsocket(mStream.GetSocket(), FIONBIO, &swt);
}

auto TcpListener::GetStreamPtr() -> TcpStream*
{
	return &mStream;
}

auto TcpListener::GetSocket() const -> const SOCKET
{
	return mStream.GetSocket();
}

auto TcpListener::GetSocket() -> SOCKET
{
	return mStream.GetSocket();
}

auto TcpListener::SetSocket(SOCKET socket) -> void
{
	mStream.SetSocket(socket);
}

auto TcpListener::GetAddrPtr() -> SOCKADDR_IN*
{
	return mStream.GetAddrPtr();
}

auto TcpListener::GetBuffer() -> WSABUF*
{
	return mStream.GetBuffer();
}

auto TcpListener::GetRecvBytes() const -> const DWORD
{
	return mStream.GetRecvBytes();
}

auto TcpListener::SetRecvBytes(DWORD size) -> void
{
	mStream.SetRecvBtyes(size);
}

auto TcpListener::GetSendBytes() const -> const DWORD
{
	return mStream.GetSendBytes();
}

auto TcpListener::SetSendBytes(DWORD size) -> void
{
	mStream.SetSendBytes(size);
}

auto TcpListener::GetOverlappedPtr() -> LPOVERLAPPED
{
	return mStream.GetOverlappedPtr();
}
