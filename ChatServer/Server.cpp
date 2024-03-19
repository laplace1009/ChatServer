#include "pch.h"
#include "Server.h"

Atomic<uint32> Server::mId = 0;

Server::Server()
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
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
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(&retOver), 1000))
	{
		client = reinterpret_cast<AsyncStream*>(retOver->GetOwner());
		std::cout << "Client sock: " << client->ConstGetSocket() << std::endl;

		switch (client->GetIOEvent())
		{
		case IOEvent::ACCEPT:
			IOAccept(client);
			return true;
		case IOEvent::RECV:
			Log("Recv OK!\n");
			return true;
		case IOEvent::SEND:
			Log("Send OK!\n");
			return true;
		case IOEvent::DISCONNECT:
			IODisconnect(client);
			return true;
		default:
			return true;
		}
	}
	else
	{
		std::cout << WSAGetLastError() << std::endl;
	}

	return false;
}

auto Server::Run(uint16 port) -> bool
{
	if (false == mListener.BindAny(port))
		return false;

	if (false == Register(&mListener.GetAsyncStreamRef()))
		return false;

	accept();

	return true;
}

auto Server::Run(std::string addr, uint16 port) -> bool
{
	if (false == mListener.Bind(addr, port))
		return false;

	if (false == Register(&mListener.GetAsyncStreamRef()))
		return false;

	accept();

	return true;
}

auto Server::GetHandle() -> HANDLE
{
	return mHandle;
}

auto Server::GetSocket() -> SOCKET
{
	return mListener.ConstGetSocket();
}

auto Server::accept() -> void
{
	const int32 acceptCount = 1;
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
	if (Register(client))
	{
		std::cout << "True\n";
	}
	client->SocketReuseAddr();
	client->GetOverlappedPtr()->SetIOEVent(IOEvent::ACCEPT);
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetRecvBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetOverlappedPtr())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			acceptRegister(client);
		}
	}
}

auto Server::setMsg(CHAR* msg, size_t size) -> bool
{
	return memcpy_s(mListener.GetSendBufRef().buf, mListener.GetSendBufRef().len, msg, size) == 0;
}

auto Server::IOConnect(AsyncStream* client) -> void
{
	
}

auto Server::IOAccept(AsyncStream* client) -> void
{
	if (mListener.SocketAcceptUpdate(client) == false)
	{
		mListener.SocketAcceptUpdate(client);
	}

	std::wstring msg{ L"Hello, World!\n" };
	IOSend(client, reinterpret_cast<CHAR*>(msg.data()), sizeof(WCHAR) * msg.length());
}

auto Server::IORecv(AsyncStream* client) -> void
{
	client->Recv();
	//std::cout << client->GetRecvBufRef().buf << std::endl;
}

auto Server::IOSend(AsyncStream* client, CHAR* msg, size_t size) -> void
{
	
	if (setMsg(msg, size) == false)
	{
		setMsg(msg, size);
	}

	for (auto& client : mClients)
	{
		client->GetOverlappedPtr()->SetIOEVent(IOEvent::SEND);
		mListener.Send(client, msg, size);
	}
}


auto Server::IODisconnect(AsyncStream* client) -> void
{

}

auto Server::Log(const char* msg) -> void
{
	std::cout << msg << std::endl;
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
