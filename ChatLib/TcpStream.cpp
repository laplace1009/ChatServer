#include "pch.h"
#include "TcpStream.h"
#include "Memory.h"

LPFN_CONNECTEX		TcpStream::LpFnConnectEx		= nullptr;
LPFN_DISCONNECTEX	TcpStream::LpFnDisconnectEx	= nullptr;
LPFN_ACCEPTEX		TcpStream::LpFnAcceptEx		= nullptr;

auto TcpStream::Init() -> bool
{
	mSocket.buf = xnew<CHAR>(MAX_BUFF_SIZE + 1);
	mSocket.wsaBuf.buf = mSocket.buf;
	mSocket.wsaBuf.len = MAX_BUFF_SIZE;
	ZeroMemory(&mSocket.overlapped.wSaOverlapped, sizeof(WSAOVERLAPPED));
	ZeroMemory(&mSocket.addr, sizeof(mSocket.addr));
	mSocket.socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mSocket.socket == SOCKET_ERROR)
		return false;

	if (bindWsaIoctl(WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&LpFnConnectEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&LpFnDisconnectEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&LpFnAcceptEx)) == false)
		return false;

	return true;
}

auto TcpStream::Close() -> void
{
	xdelete<CHAR>(mSocket.buf);
	closesocket(mSocket.socket);
}

auto TcpStream::Connect(std::string_view addr, uint16 port) -> int
{
	mSocket.addr.sin_family = AF_INET;
	inet_pton(AF_INET, addr.data(), &mSocket.addr.sin_addr);
	mSocket.addr.sin_port = htons(port);
	return connect(mSocket.socket, reinterpret_cast<SOCKADDR*>(&mSocket.addr), sizeof(mSocket.addr));
}

auto TcpStream::Recv(uint32 offset) -> int
{
	DWORD flags = 0;

	return WSARecv(mSocket.socket, &mSocket.wsaBuf, 1, &mSocket.recvBytes, OUT & flags, reinterpret_cast<LPWSAOVERLAPPED>(&mSocket.overlapped), NULL);
}

auto TcpStream::Send(CHAR* message, uint32 msgLength, uint32 offset, DWORD bufCount) -> int
{
	return WSASend(mSocket.socket, &mSocket.wsaBuf, bufCount, &mSocket.sendBytes, 0, reinterpret_cast<LPWSAOVERLAPPED>(&mSocket.overlapped), NULL);
}

auto TcpStream::SetSocketOpt(int option) -> int
{
	int optVal = true;
	int optLen = sizeof(int);

	return setsockopt(mSocket.socket, SOL_SOCKET, option, reinterpret_cast<char*>(&optVal), optLen);
}

auto TcpStream::GetSocketInfoPtr() -> SocketInfo*
{
	return &mSocket;
}

auto TcpStream::GetSocketInfoPtr() const -> const SocketInfo*
{
	return &mSocket;
}

auto TcpStream::GetMaxBuffSize() -> uint32
{
	return MAX_BUFF_SIZE;
}

auto TcpStream::bindWsaIoctl(GUID guid, LPVOID* fn) -> bool
{
	DWORD bytes = 0;
	return SOCKET_ERROR != WSAIoctl(mSocket.socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}
