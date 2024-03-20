#include "pch.h"
#include "AsyncListener.h"

AsyncListener::AsyncListener()
{
	ASSERT_CRASH(AsyncStream::Init());
}

bool AsyncListener::BindAny(uint16 port)
{
	if (mListener.BindAny(port) == false)
		return false;

	return ::listen(mListener.ConstGetSocket(), SOMAXCONN) != SOCKET_ERROR;
}

bool AsyncListener::Bind(std::string addr, uint16 port)
{
	return mListener.Bind(addr, port);
}

bool AsyncListener::Accept()
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	AsyncStream* client = xnew<AsyncStream>();
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetRecvBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{

		}
	}

	return true;
}


bool AsyncListener::Recv()
{
	return mListener.Recv();
}

bool AsyncListener::Send(CHAR* msg, size_t size)
{
	return WSASend(mListener.ConstGetSocket(), &mListener.GetSendBufRef(), 1, &mListener.GetSendBytesRef(), 0, mListener.GetOverlappedRef(), NULL);
}

auto AsyncListener::Accept(AsyncStream* client) -> bool
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	AsyncStream* newClient = static_cast<AsyncStream*>(client);
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), newClient->ConstGetSocket(), newClient->GetRecvBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(newClient->GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{

		}
	}

	return true;
}

auto AsyncListener::Send(AsyncStream* dest, CHAR* msg, size_t size) -> bool
{
	return SOCKET_ERROR != WSASend(dest->ConstGetSocket(), &mListener.GetSendBufRef(), 1, &mListener.GetSendBytesRef(), 0, mListener.GetOverlappedRef(), NULL);
}

//bool AsyncListener::SetSendMessage(Stream* client, std::wstring msg, DWORD msgSize)
//{
//	return memcpy_s(mListener.GetSendBufRef().buf, mListener.GetSendBufRef().len, msg.c_str(), msg.size() * sizeof(wchar_t)) == 0;
//}

const SOCKET AsyncListener::ConstGetSocket() const
{
	return mListener.ConstGetSocket();
}

SOCKET& AsyncListener::GetSocketRef()
{
	return mListener.GetSocketRef();
}

SOCKADDR_IN& AsyncListener::GetAddrRef()
{
	return mListener.GetAddrRef();
}

WSABUF& AsyncListener::GetRecvBufRef()
{
	return mListener.GetRecvBufRef();
}

WSABUF& AsyncListener::GetSendBufRef()
{
	return mListener.GetSendBufRef();
}

const DWORD AsyncListener::GetRecvBytes() const
{
	return mListener.GetRecvBytes();
}

const DWORD AsyncListener::GetSendBytes() const
{
	return mListener.GetSendBytes();
}

auto AsyncListener::SetSocket(SOCKET socket) -> void
{
	mListener.GetSocketRef() = socket;
}

auto AsyncListener::SetRecvBytes(DWORD bytes) -> void
{
	mListener.SetRecvBytes(bytes);
}

auto AsyncListener::SetSendBytes(DWORD bytes) -> void
{
	mListener.SetSendBytes(bytes);
}

auto AsyncListener::GetAsyncStreamRef() -> AsyncStream&
{
	return mListener;
}

auto AsyncListener::SocketAcceptUpdate(AsyncStream* client) -> bool
{
	return SetSocketOpt<SOCKET>(client, SO_UPDATE_ACCEPT_CONTEXT, &mListener.GetSocketRef(), sizeof(mListener.ConstGetSocket()));
}

auto AsyncListener::GetSendBytesRef() -> DWORD&
{
	return mListener.GetSendBytesRef();
}

auto AsyncListener::GetRecvBytesRef() -> DWORD&
{
	return mListener.GetRecvBytesRef();
}
