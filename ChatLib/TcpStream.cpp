#include "pch.h"
#include "TcpStream.h"
#include "Memory.h"

LPFN_CONNECTEX		TcpStream::LpFnConnectEx	= nullptr;
LPFN_DISCONNECTEX	TcpStream::LpFnDisconnectEx	= nullptr;
LPFN_ACCEPTEX		TcpStream::LpFnAcceptEx		= nullptr;

auto TcpStream::Send(TcpStream& stream) -> bool
{
	stream.SetIOEvent(IOEvent::SEND);
	if (WSASend(stream.GetSocket(), stream.GetBuffer(), 1, NULL, 0, stream.GetOverlappedPtr(), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			return false;
	}

	return true;
}

auto TcpStream::Recv(TcpStream& stream) -> bool
{
	DWORD flags = 0;
	stream.SetIOEvent(IOEvent::RECV);
	if (WSARecv(stream.GetSocket(), stream.GetBuffer(), 1, NULL, OUT & flags, stream.GetOverlappedPtr(), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			return false;
	}

	return true;
}

auto TcpStream::Init() -> bool
{
	mSocket.buf.buf = static_cast<char*>(PoolAllocator::Allocate(MAX_BUFF_SIZE + 1));
	mSocket.buf.len = MAX_BUFF_SIZE;
	ZeroMemory(&mSocket.overlapped.wSaOverlapped, sizeof(WSAOVERLAPPED));
	ZeroMemory(&mSocket.addr, sizeof(mSocket.addr));
	mSocket.socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	mSocket.overlapped.event = IOEvent::ACCEPT;
	
	if (mSocket.socket == INVALID_SOCKET)
	{
		PoolAllocator::Release(mSocket.buf.buf);
		return false;
	}

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
	closesocket(mSocket.socket);
}

auto TcpStream::SetAddr(std::string addr, uint16 port) -> bool
{
	mSocket.addr.sin_family = AF_INET;
	mSocket.addr.sin_port = htons(port);
	if (inet_pton(AF_INET, addr.c_str(), &mSocket.addr.sin_addr) == 1)
		return true;

	return false;
}

auto TcpStream::Bind() -> bool
{
	mSocket.addr.sin_family = AF_INET;
	mSocket.addr.sin_port = htons(0);
	mSocket.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	return bind(mSocket.socket, reinterpret_cast<PSOCKADDR>(&mSocket.addr), sizeof(SOCKADDR_IN)) == 0;
}

auto TcpStream::Connect() -> bool
{
	return TcpStream::LpFnConnectEx(mSocket.socket, reinterpret_cast<PSOCKADDR>(&mSocket.addr), sizeof(SOCKADDR_IN), NULL, NULL, &mSocket.recvBytes, reinterpret_cast<LPOVERLAPPED>(&mSocket.overlapped));
}

auto TcpStream::Connect(std::string addr, uint16 port) -> bool
{
	mSocket.addr.sin_family = AF_INET;
	inet_pton(AF_INET, addr.c_str(), &mSocket.addr.sin_addr);
	mSocket.addr.sin_port = htons(port);
	return TcpStream::LpFnConnectEx(mSocket.socket, reinterpret_cast<PSOCKADDR>(&mSocket.addr), sizeof(SOCKADDR_IN), NULL, NULL, &mSocket.recvBytes, reinterpret_cast<LPOVERLAPPED>(&mSocket.overlapped));
}

auto TcpStream::GetSocket() const -> const SOCKET
{
	return mSocket.socket;
}

auto TcpStream::GetSocket() -> SOCKET
{
	return mSocket.socket;
}

auto TcpStream::SetSocket(SOCKET socket) -> void
{
	mSocket.socket = socket;
}

auto TcpStream::GetAddrPtr() -> SOCKADDR_IN*
{
	return &mSocket.addr;
}

auto TcpStream::GetBuffer()-> WSABUF*
{
	return &mSocket.buf;
}

auto TcpStream::GetRecvBytes() const -> const DWORD
{
	return mSocket.recvBytes;
}

auto TcpStream::SetRecvBtyes(DWORD size) -> void
{
	mSocket.recvBytes = size;
}

auto TcpStream::GetSendBytes() const -> const DWORD
{
	return mSocket.sendBytes;
}

auto TcpStream::GetSendBytes() -> DWORD
{
	return mSocket.sendBytes;
}

auto TcpStream::SetSendBytes(DWORD size) -> void
{
	mSocket.sendBytes = size;
}

auto TcpStream::GetOverlappedPtr() -> LPOVERLAPPED
{
	return reinterpret_cast<LPOVERLAPPED>(&mSocket.overlapped);
}

auto TcpStream::GetIOEvent() const -> const IOEvent
{
	return mSocket.overlapped.event;
}

auto TcpStream::GetIOEvent() -> IOEvent
{
	return mSocket.overlapped.event;
}

auto TcpStream::SetIOEvent(IOEvent event) -> void
{
	mSocket.overlapped.event = event;
}

auto TcpStream::SetSocketOpt(int option) -> bool
{
	int optLen = sizeof(option);
	return setsockopt(mSocket.socket, SOL_SOCKET, option, nullptr, 0) == SOCKET_ERROR;
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
