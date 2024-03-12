#include "pch.h"
#include "AsyncListener.h"

AsyncListener::AsyncListener()
{
	ASSERT_CRASH(AsyncStream::Init());
	ASSERT_CRASH(mIocp.Init());
}

AsyncListener::~AsyncListener()
{
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
	const int32 acceptCount = 5;
	for (int32 i = 0; i < acceptCount; ++i)
	{
		AsyncStream* client = xnew<AsyncStream>();
		client->SetSocket(AsyncStream::CreateSocket());
		mClients.emplace_back(client);
		AcceptRegister(client);
	}

	return false;
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

auto AsyncListener::AcceptRegister(AsyncStream* client) -> void
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	char buf[1024]{ 0 };
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), buf, 0, addrLen, addrLen, OUT &recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetOverlappedPtr())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			AcceptRegister(client);
		}
	}
}