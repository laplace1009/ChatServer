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

	return ::listen(mListener.GetEndpointRef().ConstGetSocket(), SOMAXCONN) != SOCKET_ERROR;
}

bool AsyncListener::Bind(std::string addr, uint16 port)
{
	return mListener.Bind(addr, port);
}

bool AsyncListener::Accept()
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	AsyncEndpoint* client = xnew<AsyncEndpoint>();
	if (false == AsyncStream::AcceptEx(mListener.GetEndpointRef().ConstGetSocket(), client->GetEndpointRef().ConstGetSocket(), client->GetBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetEndpointRef().GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{

		}
	}

	return true;
}

bool AsyncListener::Connect(DWORD* bytes)
{
	return true;
}

bool AsyncListener::Recv(WSABUF* buf, DWORD* bytes)
{
	return mListener.Recv();
}

bool AsyncListener::Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)
{
	return WSASend(mListener.GetEndpointRef().ConstGetSocket(), buf, 1, bytes, 0, mListener.GetEndpointRef().GetOverlappedRef(), NULL);
}

auto AsyncListener::Accept(AsyncEndpoint* client) -> bool
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	AsyncEndpoint* newClient = static_cast<AsyncEndpoint*>(client);
	if (false == AsyncStream::AcceptEx(mListener.GetEndpointRef().ConstGetSocket(), newClient->GetEndpointRef().ConstGetSocket(), newClient->GetBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(newClient->GetEndpointRef().GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{

		}
	}

	return true;
}

auto AsyncListener::Send(AsyncEndpoint* dest, CHAR* msg, size_t size) -> bool
{
	return SOCKET_ERROR != WSASend(dest->GetEndpointRef().ConstGetSocket(), &mListener.GetBufRef(), 1, &mListener.GetTransferredBytesRef(), 0, mListener.GetEndpointRef().GetOverlappedRef(), NULL);
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
	return mListener.GetEndpointRef().GetSocketRef();
}

SOCKADDR_IN& AsyncListener::GetAddrRef()
{
	return mListener.GetEndpointRef().GetAddrRef();
}

auto AsyncListener::SetSocket(SOCKET socket) -> void
{
	mListener.GetEndpointRef().GetSocketRef() = socket;
}

auto AsyncListener::SetTransferredBytes(DWORD bytes) -> void
{
	mListener.SetTransferredBytes(bytes);
}

auto AsyncListener::GetAsyncStreamRef() -> AsyncEndpoint&
{
	return mListener;
}

auto AsyncListener::SocketAcceptUpdate(AsyncEndpoint* client) -> bool
{
	return SetSocketOpt<SOCKET>(&client->GetEndpointRef(), SO_UPDATE_ACCEPT_CONTEXT, &mListener.GetEndpointRef().GetSocketRef(), sizeof(mListener.GetEndpointRef().ConstGetSocket()));
}

auto AsyncListener::GetTransferredBytesRef() -> DWORD&
{
	return mListener.GetTransferredBytesRef();
}
