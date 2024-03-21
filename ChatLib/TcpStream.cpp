#include "pch.h"
#include "TcpStream.h"

TcpStream::TcpStream() : mSocket{0}
{
	ZeroMemory(&mAddr, sizeof(mAddr));
}

bool TcpStream::BindAny(uint16 port)
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr)) == 0;
}

bool TcpStream::Bind(std::string addr, uint16 port)
{
	TcpStream::SetAddr(addr, port);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr)) == 0;
}


auto TcpStream::Connect(DWORD* bytes) -> bool
{
	return connect(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr)) == 0;
}

bool TcpStream::Recv(WSABUF* buf, DWORD* bytes)
{
	return recv(mSocket, buf->buf, buf->len, 0) != SOCKET_ERROR;
}

bool TcpStream::Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)
{
	return send(mSocket, buf->buf, buf->len, 0) != SOCKET_ERROR;
}

const SOCKET TcpStream::ConstGetSocket() const
{
	return mSocket;
}

SOCKET& TcpStream::GetSocketRef()
{
	return mSocket;
}

SOCKADDR_IN& TcpStream::GetAddrRef()
{
	return mAddr;
}

auto TcpStream::SetAddr(std::string addr, uint16 port) -> void
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	inet_pton(AF_INET, addr.c_str(), &mAddr);
}

auto TcpStream::SocketConnectUpdate() -> bool
{
	return SetSocketOpt<int>(this, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
}

auto TcpStream::SocketReuseAddr() -> bool
{
	bool flag{ true };
	return SetSocketOpt<bool>(this, SO_REUSEADDR, &flag, sizeof(bool));
}

auto TcpStream::SocketTcpNoDelay() -> bool
{
	bool flag{ true };
	return SetSocketOpt<bool>(this, TCP_NODELAY, &flag, sizeof(bool));
}
