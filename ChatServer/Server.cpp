#include "pch.h"
#include "Server.h"

Atomic<uint32> Server::mId = 0;

Server::Server()
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(mHandle != NULL);
	mRecvBuf.buf = reinterpret_cast<CHAR*>(XALLOCATE(MAX_BUFF_SIZE));
	mRecvBuf.len = MAX_BUFF_SIZE;
}

Server::~Server() noexcept
{
	for (auto client : mClients)
	{
		xdelete<AsyncEndpoint>(client);
	}
	mClients.clear();
	XRELEASE(mRecvBuf.buf);
	mRecvBuf.buf = nullptr;
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
	OverlappedEx* retOver = nullptr;
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
		LPAsyncEndpoint client = xnew<AsyncEndpoint>();
		mClients.emplace_back(client);
		acceptRegister(client);
	}
}

auto Server::acceptRegister(LPAsyncEndpoint client) -> void
{
	Register(client);

	if (client->SocketReuseAddr() == false)
		client->SocketReuseAddr();

	if (client->SocketTcpNoDelay() == false)
		client->SocketTcpNoDelay();

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
}

auto Server::setMsg(CHAR* msg, size_t size) -> bool
{
	return memcpy_s(mRecvBuf.buf, mRecvBuf.len, msg, size) == 0;
}

auto Server::IOConnect(LPAsyncEndpoint client) -> void
{
	
}

auto Server::IOAccept(LPAsyncEndpoint client) -> void
{
	if (mListener.SocketAcceptUpdate(client) == false)
	{
		mListener.SocketAcceptUpdate(client);
	}

	client->Recv();
	client->GetEndpointRef().SetIOEvent(IOEvent::RECV);

	//std::wstring msg{ L"Hello, World!\n" };
	//IOSend(client, reinterpret_cast<CHAR*>(msg.data()), sizeof(WCHAR) * msg.length());
}

auto Server::IORecv(LPAsyncEndpoint client) -> void
{
	client->Recv();
	//std::cout << client->GetRecvBufRef().buf << std::endl;
}

auto Server::IOSend(CHAR* msg, size_t size) -> void
{
	
	if (setMsg(msg, size) == false)
	{
		setMsg(msg, size);
	}

	for (auto& client : mClients)
	{
		mListener.Send(client, msg, size);
	}
}


auto Server::IODisconnect(LPAsyncEndpoint client) -> void
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
