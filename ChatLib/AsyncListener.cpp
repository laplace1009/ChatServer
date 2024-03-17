#include "pch.h"
#include "AsyncListener.h"

AsyncListener::AsyncListener()
{
	ASSERT_CRASH(AsyncStream::Init());
	SetSocket(AsyncStream::CreateSocket());
	ASSERT_CRASH(ConstGetSocket() != SOCKET_ERROR);
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
	client->SetSocket(client->CreateSocket());
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetRecvBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetOverlappedPtr())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{

		}
	}

	return true;
}

bool AsyncListener::Accept(Stream* client)
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	AsyncStream* newClient = static_cast<AsyncStream*>(client);
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), newClient->ConstGetSocket(), newClient->GetRecvBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(newClient->GetOverlappedPtr())))
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
	return false;
}

bool AsyncListener::Send(std::wstring msg, DWORD msgLen)
{
	SetSendMessage(msg, msgLen);
	return false;
}

bool AsyncListener::SetSendMessage(std::wstring msg, DWORD msgSize)
{
	return memcpy_s(mListener.GetSendBufRef().buf, mListener.GetSendBufRef().len, msg.c_str(), msg.size() * sizeof(wchar_t)) == 0;
}

const SOCKET AsyncListener::ConstGetSocket() const
{
	return mListener.ConstGetSocket();
}

void AsyncListener::SetSocket(SOCKET socket)
{
	mListener.SetSocket(socket);
}

SOCKADDR_IN& AsyncListener::GetAddrRef()
{
	return mListener.GetAddrRef();
}

WSABUF& AsyncListener::GetRecvBufRef()
{
	return mListener.GetRecvBufRef();
}

const DWORD AsyncListener::GetRecvBytes() const
{
	return mListener.ConstGetRecvBytes();
}

WSABUF& AsyncListener::GetSendBufRef()
{
	return mListener.GetSendBufRef();
}

const DWORD AsyncListener::GetSendBytes() const
{
	return mListener.ConstGetSendBytes();
}

auto AsyncListener::GetSocketPtr() -> SOCKET*
{
	return mListener.GetSocketPtr();
}

auto AsyncListener::GetAsyncStreamRef() -> AsyncStream&
{
	return mListener;
}

auto AsyncListener::SocketAcceptUpdate(AsyncStream* client) -> bool
{
	return SetSocketOpt<SOCKET>(client, SO_UPDATE_ACCEPT_CONTEXT, mListener.GetSocketPtr(), sizeof(mListener.ConstGetSocket()));
}
