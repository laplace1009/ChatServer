#include "pch.h"
#include "Server.h"

Atomic<uint32> Server::mId = 0;

Server::Server()
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(mHandle != NULL);
	ASSERT_CRASH(AsyncStream::Init() == Error::OK);
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

Error Server::Register(LPAsyncEndpoint socket)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket->ConstGetSocket()), mHandle, 0, 0))
		return Error::IOCP_REGISTER_ERROR;

	return Error::OK;
}

Error Server::Dispatch()
{
	ULONG_PTR key;
	DWORD transferred = 0;
	LPOVERLAPPEDEX retOver = nullptr;
	LPAsyncEndpoint client = nullptr;
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(&retOver), 100))
	{
		client = reinterpret_cast<LPAsyncEndpoint>(retOver->owner);
		std::cout << "IOCP Server" << std::endl;
		doIOAction(client);
	}
	else
	{
		int32 error = WSAGetLastError();
		switch (error)
		{
		case WAIT_TIMEOUT:
			return Error::OK;
		default:
			return Error::IOCP_DISPATCH_ERROR;
		}
	}
	return Error::OK;
}

auto Server::Run(uint16 port) -> Error
{
	Error error = mListener.BindAny(port);
	if (Error::NET_BIND_ERROR == error)
		return Error::NET_BIND_ERROR;

	if (Error::NET_LISTEN_ERROR == error)
		return Error::NET_LISTEN_ERROR;

	if (Error::IOCP_REGISTER_ERROR == Register(&mListener.GetAsyncStreamRef()))
		return Error::IOCP_REGISTER_ERROR;

	accept();

	return Error::OK;
}

auto Server::Run(std::string addr, uint16 port) -> Error
{
	Error error = mListener.Bind(addr, port);
	if (Error::NET_BIND_ERROR == error)
		return Error::NET_BIND_ERROR;

	if (Error::NET_LISTEN_ERROR == error)
		return Error::NET_LISTEN_ERROR;

	if (Error::IOCP_REGISTER_ERROR == Register(&mListener.GetAsyncStreamRef()))
		return Error::IOCP_REGISTER_ERROR;

	accept();

	return Error::OK;
}

auto Server::GetHandle() -> HANDLE
{
	return mHandle;
}

auto Server::GetSocket() -> SOCKET
{
	return mListener.ConstGetSocket();
}

auto Server::Send() -> Error
{
	while (mSendBuffs.empty() == false)
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
				return Error::SERVER_EMPTY_SENDBUFFS;
			}
		}

		{
			WriteLockGuard<ReadWriteLock&> g(mLock);

			for (size_t i = 0; i < mClients.size() - ACCEPT_COUNT; ++i)
			{
				mClients[i]->Send(sendBuf);
			}
		}
	}
	
	return Error::OK;
}

auto Server::SetRecv(LPAsyncEndpoint client) -> Error
{
	if (client->Recv() == Error::NET_RECV_ERROR)
		return Error::NET_RECV_ERROR;
	setEventRecv(client);

	return Error::OK;
}

auto Server::accept() -> void
{
	const int32 acceptCount = ACCEPT_COUNT;
	for (int32 i = 0; i < acceptCount; ++i)
	{
		LPAsyncEndpoint client = xnew<AsyncEndpoint>();
		ASSERT_CRASH(Register(client) != Error::IOCP_REGISTER_ERROR);
		{
			WriteLockGuard<ReadWriteLock&> g(mLock);
			mClients.emplace_back(client);
		}
		acceptRegister(client);
	}
}

auto Server::acceptRegister() -> void
{
	DWORD addrLen{ sizeof(SOCKADDR_IN) + 16 };
	DWORD recvBytes{ 0 };
	LPAsyncEndpoint client = new AsyncEndpoint();
	while (Register(client) == Error::IOCP_REGISTER_ERROR);
	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetEndpointRef().GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			acceptRegister(client);
			return;
		}
	}
	setEventAccept(client);

	{
		WriteLockGuard<ReadWriteLock&> g(mLock);
		mClients.emplace_back(client);
	}
}

auto Server::acceptRegister(LPAsyncEndpoint client) -> void
{
	DWORD addrLen{ sizeof(SOCKADDR_IN) + 16 };
	DWORD recvBytes{ 0 };

	if (false == AsyncStream::AcceptEx(mListener.ConstGetSocket(), client->ConstGetSocket(), client->GetBufRef().buf, 0, addrLen, addrLen, OUT & recvBytes, OUT static_cast<LPOVERLAPPED>(client->GetEndpointRef().GetOverlappedRef())))
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			acceptRegister(client);
			return;
		}
	}
	setEventAccept(client);
}

auto Server::setMsg(WSABUF* dst, LPAsyncEndpoint src) -> void
{
	while (memcpy_s(dst->buf, MAX_BUFF_SIZE, src->GetBufRef().buf, src->GetTransferredBytes()) != 0);
	dst->len = src->GetTransferredBytes();
}

auto Server::doIOAction(LPAsyncEndpoint client) -> void
{
	IOEvent ioEvent = client->GetEndpointRef().GetIOEvent();
	switch (ioEvent)
	{
	case IOEvent::ACCEPT:
		afterIOAcceptEvent(client);
		return;
	case IOEvent::RECV:
		afterIORecvEvent(client);
		return;
	case IOEvent::SEND:
		afterIOSendEvent(client);
		return;
	case IOEvent::DISCONNECT:
		afterIODisconnect(client);
		return;
	default:
		CRASH("Do not reach this case");
		return;
	}
}

auto Server::setEventAccept(LPAsyncEndpoint client) -> void
{
	client->GetEndpointRef().SetIOEvent(IOEvent::ACCEPT);
}

auto Server::setEventRecv(LPAsyncEndpoint client) -> void
{
	client->GetEndpointRef().SetIOEvent(IOEvent::RECV);
}

auto Server::setEventSend(LPAsyncEndpoint client) -> void
{
	client->GetEndpointRef().SetIOEvent(IOEvent::SEND);
}

auto Server::setEventDisconnect(LPAsyncEndpoint client) -> void
{
	client->GetEndpointRef().SetIOEvent(IOEvent::DISCONNECT);
}

auto Server::afterIOAcceptEvent(LPAsyncEndpoint client) -> void
{
	mListener.SocketAcceptUpdate(client);
	setEventAccept(client);
	SetRecv(client);
	acceptRegister();
}

auto Server::afterIORecvEvent(LPAsyncEndpoint client) -> void
{
	WSABUF* buf = xnew<WSABUF>();
	PacketHeader header = *(reinterpret_cast<PacketHeader*>(client->GetBufRef().buf));
	buf->buf = new char[1024];
	memcpy_s(buf->buf, 1024, client->GetBufRef().buf, header.size);
	buf->len = header.size;
	ZeroMemory(client->GetBufRef().buf, buf->len);
	client->GetTransferredBytesRef() = 0;
	{
		WriteLockGuard<ReadWriteLock&> g(mLock);
		mSendBuffs.emplace(buf);
	}
	
	Send();
}

auto Server::afterIOSendEvent(LPAsyncEndpoint client) -> void
{
	setEventRecv(client);
	client->GetTransferredBytesRef() = 0;
}

auto Server::afterIODisconnect(LPAsyncEndpoint client) -> void
{
	setEventDisconnect(client);
}

