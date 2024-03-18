#include "pch.h"
#include "Server.h"

Atomic<uint32> Server::mId = 0;

Server::Server()
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	ASSERT_CRASH(mHandle != NULL);
}

Server::~Server() noexcept
{
	for (auto client : mClients)
	{
		xdelete<AsyncStream>(client);
	}
	mClients.clear();
}

bool Server::Register(AsyncStream* stream)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(stream->ConstGetSocket()), mHandle, 0, 0))
		return false;
		
	return true;
}

bool Server::Dispatch()
{
	ULONG_PTR key;
	DWORD transferred = 0;
	OverlappedEx* retOver = nullptr;
	AsyncStream* client = nullptr;
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(&retOver), INFINITE))
	{
		client = reinterpret_cast<AsyncStream*>(retOver->GetOwner());
		switch (client->GetIOEvent())
		{
		case IOEvent::CONNECT:
			IOConnect(client);
			break;
		case IOEvent::RECV:
			IORecv(client);
			break;
		case IOEvent::SEND:
			//IOSend(client);
			break;
		case IOEvent::DISCONNECT:
			IODisconnect(client);
			break;
		default:
			break;
		}
	}

	return true;
}

auto Server::Run(uint16 port) -> bool
{
	if (NULL == ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(mListener.ConstGetSocket()), mHandle, (ULONG_PTR)mListener.ConstGetSocket(), 0))
		return false;

	if (false == mListener.BindAny(port))
		return false;
	
	accept();

	return true;
}

auto Server::Run(std::string addr, uint16 port) -> bool
{
	if (NULL == ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(mListener.ConstGetSocket()), mHandle, (ULONG_PTR)mListener.ConstGetSocket(), 0))
		return false;

	if (false == mListener.Bind(addr, port))
		return false;

	accept();

	return true;
}

auto Server::GetHandle() -> HANDLE
{
	return mHandle;
}

auto Server::accept() -> void
{
	const int32 acceptCount = 5;
	for (int32 i = 0; i < acceptCount; ++i)
	{
		AsyncStream* client = xnew<AsyncStream>();
		client->SetSocket(AsyncStream::CreateSocket());
		mClients.emplace_back(client);
		acceptRegister(client);
	}
}

auto Server::acceptRegister(AsyncStream* client) -> void
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetRecvBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetOverlappedPtr())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			acceptRegister(client);
		}
	}
}

auto Server::IOConnect(AsyncStream* client) -> void
{
	IOAccept(client);
	mListener.GetAsyncStreamRef().GetOverlappedPtr()->SetIOEVent(IOEvent::ACCEPT);
	IOSend(client, L"Hello, World");
}

auto Server::IOAccept(AsyncStream* client) -> void
{
	if (mListener.SocketAcceptUpdate(client) == false)
	{
		mListener.SocketAcceptUpdate(client);
	}

}

auto Server::IORecv(AsyncStream* client) -> void
{

}

auto Server::IOSend(AsyncStream* client, std::wstring msg) -> void
{
	mListener.Send(static_cast<Stream*>(client), msg, msg.length() * sizeof(WCHAR));
}

auto Server::IODisconnect(AsyncStream* client) -> void
{

}


//Iocp::~Iocp() noexcept
//{
//	CloseHandle(mHandle);
//}
//
//auto Iocp::Init() -> bool
//{
//	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//	if (mHandle == NULL)
//		return false;
//
//	return true;
//}
//
//auto Iocp::GetHandle() -> HANDLE
//{
//	return mHandle;
//}
//
//auto Iocp::SetHandle(HANDLE h) -> void
//{
//	mHandle = h;
//}
//
//auto Iocp::Register(AsyncStream& stream) -> bool
//{
//	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(stream.ConstGetSocket()), mHandle, reinterpret_cast<ULONG_PTR>(&stream), 0))
//		return false;
//
//	return true;
//}
//
//auto Iocp::Dispatch(uint32 timeout) -> bool
//{
//
//
//	return true;
//}
