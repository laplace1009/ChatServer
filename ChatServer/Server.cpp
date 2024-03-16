#include "pch.h"
#include "Server.h"

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
	//SOCKADDR_IN* localAddr = nullptr;
	//SOCKADDR_IN* remoteAddr = nullptr;
	//int localAddrLen = 0, remoteAddrLen = 0;
	//char remoteAddrStr[INET_ADDRSTRLEN];
	AsyncStream* client = nullptr;
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(client->GetLPOverlappedPtr()), INFINITE))
	{
		client = retOver->GetOwner();
		
		//AsyncStream::GetAcceptExSockaddrs(client->GetRecvBufRef().buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (SOCKADDR**)&localAddr, &localAddrLen, (SOCKADDR**)&remoteAddr, &remoteAddrLen);
		//inet_ntop(AF_INET, &remoteAddr->sin_addr, remoteAddrStr, sizeof(remoteAddrStr));
		//std::cout << "connect Addr: " << remoteAddrStr << std::endl;
		//std::cout << "Listen sock: " << mListener.ConstGetSocket() << std::endl;
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
