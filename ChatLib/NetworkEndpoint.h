#pragma once
#include "Types.h"
#include "TcpStream.h"
#include "AsyncStream.h"

template<typename T>
class NetworkEndpoint
{
public:
	NetworkEndpoint() : mTransferredBytes{ 0 }
	{
		mBuf.buf = reinterpret_cast<CHAR*>(XALLOCATE(MAX_BUFF_SIZE));
	}
	~NetworkEndpoint()
	{
		XRELEASE(mBuf.buf);
		mBuf.buf = nullptr ;
	}

public:
	auto BindAny(uint16 port) -> bool
	{
		return mEndPoint.BindAny(port);
	}

	auto Bind(String addr, uint16 port) -> bool
	{
		return mEndPoint.Bind(addr, port);
	}

	auto Connect() -> bool
	{
		return mEndPoint.Connect(&mTransferredBytes);
	}

	auto Recv() -> bool
	{
		return mEndPoint.Recv(&mBuf, &mTransferredBytes);
	}

	auto Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size) -> bool
	{
		return mEndPoint.Send(&mBuf, &mTransferredBytes, msg, size);
	}

public:
	auto ConstGetSocket() const -> const SOCKET
	{
		return mEndPoint.ConstGetSocket();
	}

	auto SetSocket(SOCKET socket) -> void
	{
		mEndPoint.GetSocketRef() = socket;
	}

	auto SetAddr(String addr, uint16 port) -> bool
	{
		return mEndPoint.SetAddr(addr, port);
	}

public:
	auto ConstGetEndPointRef() const -> const T&
	{
		return mEndPoint;
	}

	auto GetEndpointRef() -> T&
	{
		return mEndPoint;
	}

	auto GetBufRef() -> WSABUF&
	{
		return mBuf;
	}

	auto GetTransferredBytes() const -> DWORD
	{
		return mTransferredBytes;
	}

	auto GetTransferredBytesRef() -> DWORD&
	{
		return mTransferredBytes;
	}

	auto SetTransferredBytes(DWORD bytes) -> void
	{
		mTransferredBytes = bytes;
	}

public:
	auto SocketConnectUpdate() -> bool
	{
		return mEndPoint.SocketConnectUpdate();
	}

	auto SocketReuseAddr() -> bool
	{
		return mEndPoint.SocketReuseAddr();
	}

	auto SocketTcpNoDelay() -> bool
	{
		return mEndPoint.SocketTcpNoDelay();
	}
private:
	T mEndPoint;
	WSABUF mBuf;
	DWORD mTransferredBytes;
};

using TcpEndpoint = NetworkEndpoint<TcpStream>;
using LPTcpEndPoint = NetworkEndpoint<TcpStream>*;
using AsyncEndpoint = NetworkEndpoint<AsyncStream>;
using LPAsyncEndpoint = NetworkEndpoint<AsyncStream>*;
