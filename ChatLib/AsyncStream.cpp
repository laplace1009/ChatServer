#include "pch.h"
#include "AsyncStream.h"

LPFN_CONNECTEX				AsyncStream::ConnectEx = nullptr;
LPFN_DISCONNECTEX			AsyncStream::DisconnectEx = nullptr;
LPFN_ACCEPTEX				AsyncStream::AcceptEx = nullptr;
LPFN_GETACCEPTEXSOCKADDRS	AsyncStream::GetAcceptExSockaddrs = nullptr;

AsyncStream::AsyncStream() : mOverlapped{ xnew<OVERLAPPEDEX>() }, mSocket{ CreateSocket() }
{
	ASSERT_CRASH(mSocket != INVALID_SOCKET);
	ZeroMemory(mOverlapped, sizeof(OVERLAPPEDEX));
	ZeroMemory(&mAddr, sizeof(mAddr));
	mOverlapped->ioEvent = IOEvent::CONNECT;
	ASSERT_CRASH(SocketReuseAddr() != Error::OK);
	ASSERT_CRASH(SocketTcpNoDelay() != Error::OK);
}

AsyncStream::~AsyncStream() noexcept
{
	xdelete(mOverlapped);
}

auto AsyncStream::Init() -> Error
{	
	if (bindWsaIoctl(WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)) == false)
		return Error::NET_BIND_LPFN_ERROR;

	if (bindWsaIoctl(WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)) == false)
		return Error::NET_BIND_LPFN_ERROR;

	if (bindWsaIoctl(WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)) == false)
		return Error::NET_BIND_LPFN_ERROR;

	if (bindWsaIoctl(WSAID_GETACCEPTEXSOCKADDRS, reinterpret_cast<LPVOID*>(&GetAcceptExSockaddrs)) == false)
		return Error::NET_BIND_LPFN_ERROR;
	
	return Error::OK;
}

Error AsyncStream::BindAny(uint16 port)
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(SOCKADDR_IN)) == 0)
		return Error::OK;

	return Error::NET_BIND_ERROR;
}

Error AsyncStream::Bind(std::string addr, uint16 port)
{
	SetAddr(addr, port);
	if (bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(SOCKADDR_IN)) == 0)
		return Error::OK;

	return Error::NET_BIND_ERROR;
}

Error AsyncStream::Connect(DWORD* bytes)
{
	return AsyncStream::ConnectEx(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr), nullptr, 0, bytes, static_cast<LPOVERLAPPED>(mOverlapped)) ? Error::OK : Error::NET_CONNECT_ERROR;
}

Error AsyncStream::Recv(WSABUF* buf, DWORD* bytes)
{
	DWORD flags = 0;
	if (WSARecv(mSocket, buf, 1, bytes, OUT & flags, static_cast<LPOVERLAPPED>(mOverlapped), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			std::cout << error << std::endl;
			return Error::NET_RECV_ERROR;
		}
			
	}

	return Error::OK;
}

Error AsyncStream::Send(WSABUF* sendBuf, DWORD* bytes, CHAR* msg, size_t size)
{
	if (WSASend(mSocket, sendBuf, 1, bytes, 0, static_cast<LPOVERLAPPED>(mOverlapped), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			std::cout << error << std::endl;
			return Error::NET_SEND_ERROR;
		}
			
	}

	return Error::OK;
}

const SOCKET AsyncStream::ConstGetSocket() const
{
	return mSocket;
}

SOCKET& AsyncStream::GetSocketRef()
{
	return mSocket;
}

SOCKADDR_IN& AsyncStream::GetAddrRef()
{
	return mAddr;
}

auto AsyncStream::GetOverlappedRef() -> LPOVERLAPPEDEX&
{
	return mOverlapped;
}

auto AsyncStream::GetLPOverlappedPtr()-> LPOVERLAPPEDEX*
{
	return &mOverlapped;
}

auto AsyncStream::GetIOEvent() -> IOEvent
{
	return mOverlapped->ioEvent;
}

auto AsyncStream::SetIOEvent(IOEvent ioEvent) -> void
{
	mOverlapped->ioEvent = ioEvent;
}

auto AsyncStream::SetAddr(std::string addr, uint16 port) -> bool
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	return inet_pton(AF_INET, addr.c_str(), &mAddr.sin_addr) == 1;
}

auto AsyncStream::setMsg(WSABUF& dest, CHAR* msg, size_t size) -> bool
{
	errno_t error = memcpy_s(dest.buf, dest.len, msg, size);
	return error == 0;
}

auto AsyncStream::bindWsaIoctl(GUID guid, LPVOID* fn) -> bool
{
	DWORD bytes = 0;
	SOCKET dummySocket = CreateSocket();
	return SOCKET_ERROR != WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

auto AsyncStream::SocketConnectUpdate() -> Error
{
	return SetSocketOpt<int>(this, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
}

auto AsyncStream::SocketReuseAddr() -> Error
{
	bool flag{ true };
	return SetSocketOpt<bool>(this, SO_REUSEADDR, &flag, sizeof(bool));
}

auto AsyncStream::SocketTcpNoDelay() -> Error
{
	bool flag{ true };
	return SetSocketOpt<bool>(this, TCP_NODELAY, &flag, sizeof(bool));
}

