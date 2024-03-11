#include "pch.h"
#include "AsyncStream.h"

LPFN_CONNECTEX				AsyncStream::ConnectEx = nullptr;
LPFN_DISCONNECTEX			AsyncStream::DisconnectEx = nullptr;
LPFN_ACCEPTEX				AsyncStream::AcceptEx = nullptr;
LPFN_GETACCEPTEXSOCKADDRS	AsyncStream::GetAcceptExSockaddrs = nullptr;

AsyncStream::AsyncStream() : mOverlapped{xnew<OverlappedEx>()}, mSocket{0}, mRecvBytes{0}, mSendBytes{0}
{
	ZeroMemory(&mAddr, sizeof(mAddr));
}

AsyncStream::~AsyncStream() noexcept
{
	xdelete(mOverlapped);
}

auto AsyncStream::Init() -> bool
{	
	if (bindWsaIoctl(WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_GETACCEPTEXSOCKADDRS, reinterpret_cast<LPVOID*>(&GetAcceptExSockaddrs)) == false)
		return false;
	
	return false;
}

bool AsyncStream::CreateSocket()
{
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	return mSocket != INVALID_SOCKET;
}

bool AsyncStream::Bind()
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(0);
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(SOCKADDR_IN)) == 0;
}

bool AsyncStream::Connect()
{
	return AsyncStream::ConnectEx(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr), nullptr, 0, &mRecvBytes, reinterpret_cast<LPOVERLAPPED>(mOverlapped));
}

bool AsyncStream::Recv()
{
	DWORD flags = 0;
	mOverlapped->SetIOEVent(IOEvent::RECV);
	if (WSARecv(mSocket, &mBuf, 1, NULL, OUT & flags, static_cast<LPOVERLAPPED>(mOverlapped), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			return false;
	}

	return true;
}

bool AsyncStream::Send()
{
	mOverlapped->SetIOEVent(IOEvent::SEND);
	if (WSASend(mSocket, &mBuf, 1, NULL, 0, static_cast<LPOVERLAPPED>(mOverlapped), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			return false;
	}

	return true;
	return false;
}


auto AsyncStream::GetSocket() -> SOCKET
{
	return mSocket;
}

auto AsyncStream::GetOverlappedPtr() -> OverlappedEx*
{
	return mOverlapped;
}

auto AsyncStream::bindWsaIoctl(GUID guid, LPVOID* fn) -> bool
{
	DWORD bytes = 0;
	SOCKET dummySocket = CreateSocket();
	return SOCKET_ERROR != WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

