#include "pch.h"
#include "Server.h"
#include <iostream>
#include <thread>

auto Server::Start(uint16 port) -> bool
{
	if (mSocket.Init() == false)
		return false;

	if (init(port) == false)
		return false;

	if (mIocp.Init() == false)
		return false;

	return true;
}

auto Server::Start(std::string addr, uint16 port) -> bool
{
	// Error 따로 정의 해서 내보내기
	if (mSocket.Init() == false)
		return false;

	if (init(addr, port) == false)
		return false;

	if (mIocp.Init() == NULL)
		return false;

	return true;
}

auto Server::Close() -> bool
{
	WSACleanup();
	return false;
}

auto Server::IOAction() -> bool
{
	DWORD transferredBytes;
	OVERLAPPEDEX* overlapped{ new OVERLAPPEDEX() };
	TcpStream client;
	client.Init();
	bool ret = GetQueuedCompletionStatus(mIocp.GetHandle(), &transferredBytes, reinterpret_cast<PULONG_PTR>(this), reinterpret_cast<LPOVERLAPPED*>(&overlapped), 10000);
	if (ret)
	{
		if (overlapped->event == IOEvent::ACCEPT)
		{
			while (false == TcpStream::LpFnAcceptEx(mSocket.GetSocket(), client.GetSocket(), client.GetBuffer()->buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, client.GetOverlappedPtr()));

			{
				WriteLockGuard<ReadWriteLock&> guard(mLock);
				while (false == mIocp.Register(client));
				mClients.emplace_back(std::move(client));
			}

			SOCKADDR_IN addr;
			int addrLen = sizeof(SOCKADDR_IN);
			char addrBuf[INET_ADDRSTRLEN];
			getpeername(client.GetSocket(), reinterpret_cast<PSOCKADDR>(&addr), &addrLen);
			inet_ntop(AF_INET, &addr, addrBuf, sizeof(addrBuf));
			std::cout << "Client connec to Server, Ip: " << addrBuf << " Port: " << ntohs(addr.sin_port) << std::endl;
		}
		else if (overlapped->event == IOEvent::RECV)
		{

		}
		else if (overlapped->event == IOEvent::SEND)
		{

		}
		else if (overlapped->event == IOEvent::DISCONNECT)
		{

		}
	}

	return false;
}

auto Server::Accept() -> void
{
	int32 addrLen = sizeof(SOCKADDR_IN);
	TcpStream client;
	client.Init();
	client.GetSocketInfoPtr()->socket = ::accept(mSocket.GetSocket(), (SOCKADDR*)mSocket.GetAddrPtr(), &addrLen);
	getpeername(client.GetSocket(), (SOCKADDR*)client.GetAddrPtr(), &addrLen);
	char name[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, client.GetAddrPtr(), name, sizeof(name));
	std::cout << "ip: " << name << ", port: " << ntohs(client.GetAddrPtr()->sin_port) << std::endl;
}

auto Server::Join(TcpStream&& stream) -> bool
{
	if (mIocp.Register(stream))
	{
		mClients.emplace_back(stream);
		return true;
	}

	return false;
}

auto Server::init(uint16 port) -> bool
{	
	mSocket.BindAny(port);

	return true;
}

auto Server::init(std::string addr, uint16 port) -> bool
{
	mSocket.Bind(addr, port);

	return true;
}
