#include "pch.h"
#include "AsyncStream.h"

LPFN_CONNECTEX				AsyncStream::ConnectEx = nullptr;
LPFN_DISCONNECTEX			AsyncStream::DisconnectEx = nullptr;
LPFN_ACCEPTEX				AsyncStream::AcceptEx = nullptr;
LPFN_GETACCEPTEXSOCKADDRS	AsyncStream::GetAcceptExSockaddrs = nullptr;

AsyncStream::AsyncStream() : mOverlapped{ xnew<OverlappedEx>() }, mSocket{ 0 }, mRecvBytes { 0 }, mSendBytes{ 0 }
{
	ZeroMemory(&mAddr, sizeof(mAddr));
	mRecvBuf.buf = static_cast<CHAR*>(XALLOCATE(2048));
	mRecvBuf.len = MAX_BUFF_SIZE;
	mSendBuf.buf = static_cast<CHAR*>(XALLOCATE(2048));
	mSendBuf.len = MAX_BUFF_SIZE;
}

AsyncStream::~AsyncStream() noexcept
{
	xdelete(mOverlapped);
	XRELEASE(mRecvBuf.buf);
	mRecvBuf.buf = nullptr;
	mRecvBuf.len = 0;
	XRELEASE(mSendBuf.buf);
	mSendBuf.buf = nullptr;
	mSendBuf.len = 0;
}

auto AsyncStream::Init() -> bool
{	
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	if (bindWsaIoctl(WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)) == false)
		return false;

	if (bindWsaIoctl(WSAID_GETACCEPTEXSOCKADDRS, reinterpret_cast<LPVOID*>(&GetAcceptExSockaddrs)) == false)
		return false;
	
	return true;
}

auto AsyncStream::CreateSocket() -> SOCKET
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool AsyncStream::BindAny(uint16 port)
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(SOCKADDR_IN)) == 0;
}

bool AsyncStream::Bind(std::string addr, uint16 port)
{
	SetAddr(addr, port);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(SOCKADDR_IN)) == 0;
}

bool AsyncStream::Connect()
{
	return AsyncStream::ConnectEx(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr), nullptr, 0, &mRecvBytes, static_cast<LPOVERLAPPED>(mOverlapped));
}

bool AsyncStream::Recv()
{
	DWORD flags = 0;
	mOverlapped->SetIOEVent(IOEvent::RECV);
	if (WSARecv(mSocket, &mRecvBuf, 1, NULL, OUT & flags, static_cast<LPOVERLAPPED>(mOverlapped), NULL) == SOCKET_ERROR)
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
	if (WSASend(mSocket, &mSendBuf, 1, NULL, 0, static_cast<LPOVERLAPPED>(mOverlapped), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			return false;
	}

	return true;
	return false;
}

const SOCKET AsyncStream::ConstGetSocket() const
{
	return mSocket;
}

void AsyncStream::SetSocket(SOCKET socket)
{
	mSocket = socket;
}

const SOCKADDR_IN& AsyncStream::ConstGetAddrRef() const
{
	return mAddr;
}

SOCKADDR_IN& AsyncStream::GetAddrRef()
{
	return mAddr;
}

bool AsyncStream::SetAddr(std::string addr, uint16 port)
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	return inet_pton(AF_INET, addr.c_str(), &mAddr.sin_addr) == 1;
}

const WSABUF& AsyncStream::ConstGetRecvBufRef() const
{
	return mRecvBuf;
}

WSABUF& AsyncStream::GetRecvBufRef()
{
	return mRecvBuf;
}

const DWORD AsyncStream::ConstGetRecvBytes() const
{
	return mRecvBytes;
}

void AsyncStream::SetRecvBytes(DWORD bytes)
{
	mRecvBytes = bytes;
}

const WSABUF& AsyncStream::ConstGetSendBufRef() const
{
	return mSendBuf;
}

WSABUF& AsyncStream::GetSendBufRef()
{
	return mSendBuf;
}

const DWORD AsyncStream::ConstGetSendBytes() const
{
	return mSendBytes;
}

void AsyncStream::SetSendBytes(DWORD bytes)
{
	mSendBytes = bytes;
}

auto AsyncStream::GetOverlappedPtr() -> OverlappedEx*
{
	return mOverlapped;
}

auto AsyncStream::GetLPOveralppedPtr()-> OverlappedEx**
{
	return &mOverlapped;
}

auto AsyncStream::bindWsaIoctl(GUID guid, LPVOID* fn) -> bool
{
	DWORD bytes = 0;
	SOCKET dummySocket = CreateSocket();
	return SOCKET_ERROR != WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

