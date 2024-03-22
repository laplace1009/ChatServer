#include "pch.h"
#include "Server.h"

Atomic<uint32> Server::mId = 0;

Server::Server()
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(mHandle != NULL);
	ASSERT_CRASH(AsyncStream::Init());
}

Server::~Server() noexcept
{
	for (auto client : mClients)
	{
		xdelete<AsyncEndpoint>(client);
		client = nullptr;
	}
	mClients.clear();

	while (mSendBuffs.empty() == false)
	{
		WSABUF* buf = mSendBuffs.front();
		mSendBuffs.pop();
		delete[] buf->buf;
		buf->buf = nullptr;
		delete buf;
		buf = nullptr;
	}
}

bool Server::Register(LPAsyncEndpoint socket)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket->ConstGetSocket()), mHandle, 0, 0))
		return false;

	return true;
}

bool Server::Dispatch()
{
	ULONG_PTR key;
	DWORD transferred = 0;
	LPOVERLAPPEDEX retOver = nullptr;
	LPAsyncEndpoint client = nullptr;
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(&retOver), 1000))
	{
		client = reinterpret_cast<LPAsyncEndpoint>(retOver->owner);
		std::cout << "Client sock: " << client->ConstGetSocket() << std::endl;

		switch (client->GetEndpointRef().GetIOEvent())
		{
		case IOEvent::ACCEPT:
			IOAccept(client);
			return true;
		case IOEvent::RECV:
			Log("Recv OK!\n");
			IORecv(client);
			return true;
		case IOEvent::SEND:
			Log("Send OK!\n");
			IOSend(client);
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
		int error = WSAGetLastError();
		if (error == 258)
		{
			Log("Not Notify");
		}
		std::cout << error << std::endl;
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

auto Server::Send() -> bool
{
	WSABUF* sendBuf = nullptr;
	{
		WriteLockGuard<ReadWriteLock&> g(mLock);
		if (mSendBuffs.empty() == false)
		{
			sendBuf = mSendBuffs.front();
			mSendBuffs.pop();
		}
		else
		{
			return false;
		}
	}

	{
		WriteLockGuard<ReadWriteLock&> g(mLock);
		for (auto& client : mClients)
		{
			client->Send(sendBuf);
		}
	}
	
	return true;
}

auto Server::SetRecv(LPAsyncEndpoint client) -> bool
{
	if (client->Recv() == false)
		return false;

	return true;
}

auto Server::accept() -> void
{
	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; ++i)
	{
		LPAsyncEndpoint client = xnew<AsyncEndpoint>();
		ASSERT_CRASH(Register(client));
		mClients.emplace_back(client);
		acceptRegister(client);
	}
}

auto Server::acceptRegister(LPAsyncEndpoint client) -> void
{
	DWORD addrLen = sizeof(SOCKADDR_IN) + 16;
	DWORD recvBytes{ 0 };

	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetEndpointRef().GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			acceptRegister(client);
		}
	}
	setEventAccept(client);
}

auto Server::setMsg(WSABUF* dst, LPAsyncEndpoint src) -> void
{
	while (memcpy_s(dst->buf, MAX_BUFF_SIZE, src->GetBufRef().buf, src->GetTransferredBytes()) != 0);
	dst->len = src->GetTransferredBytes();
}

auto Server::IOAccept(LPAsyncEndpoint client) -> void
{
	if (mListener.SocketAcceptUpdate(client) == false)
	{
		mListener.SocketAcceptUpdate(client);
	}

	if (SetRecv(client) == false)
		SetRecv(client);
}

auto Server::IORecv(LPAsyncEndpoint src) -> void
{
	WSABUF* sendBuf = new WSABUF();
	sendBuf->buf = static_cast<CHAR*>(XALLOCATE(MAX_BUFF_SIZE));
	setMsg(sendBuf, src);
	src->GetBufRef().len = MAX_BUFF_SIZE;
	{
		WriteLockGuard<ReadWriteLock&> g(mLock);
		mSendBuffs.emplace(sendBuf);
	}
	Send();
}

auto Server::IOSend(LPAsyncEndpoint client) -> void
{
	// Send 완료 통보후 다시 Recv로 바꿔서 받을수 있는 준비를 한다.
	client->GetEndpointRef().SetIOEvent(IOEvent::RECV);
}


auto Server::IODisconnect(LPAsyncEndpoint client) -> void
{
	if (AsyncStream::DisconnectEx(client->ConstGetSocket(), NULL, 0, 0) == false)
	{
		int error = WSAGetLastError();
		std::cerr << "DisconnectEx failed: " << error << std::endl;
	}
}

auto Server::Log(const char* msg) -> void
{
	std::cout << msg << std::endl;
}

auto Server::setEventAccept(LPAsyncEndpoint client) -> void
{
	client->GetEndpointRef().SetIOEvent(IOEvent::ACCEPT);
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
