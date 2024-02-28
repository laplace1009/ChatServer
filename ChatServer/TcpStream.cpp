#include "pch.h"
#include "TcpStream.h"

TcpStream::TcpStream()
{
	mSocket.buf = new CHAR[MAX_BUFF_SIZE + 1];
	mSocket.wsaBuf.buf = mSocket.buf;
	mSocket.wsaBuf.len = MAX_BUFF_SIZE;
	memset(&mSocket.overlapped, 0, sizeof(mSocket.overlapped));
	memset(&mSocket.addr, 0, sizeof(mSocket.addr));
	mSocket.socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket.socket == INVALID_SOCKET)
		CRASH("WSASocket()");
}

TcpStream::~TcpStream() noexcept
{
	delete[] mSocket.buf;
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

	return WSARecv(mSocket.socket, &mSocket.wsaBuf, 1, &mSocket.recvBytes, OUT & flags, &mSocket.overlapped, NULL);
}

auto TcpStream::Send(CHAR* message, uint32 msgLength, uint32 offset, DWORD bufCount) -> int
{
	return WSASend(mSocket.socket, &mSocket.wsaBuf, bufCount, &mSocket.sendBytes, 0, &mSocket.overlapped, NULL);
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

auto TcpStream::GetMaxBuffSize() -> uint32
{
	return MAX_BUFF_SIZE;
}
