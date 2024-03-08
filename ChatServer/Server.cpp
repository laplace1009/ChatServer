#include "pch.h"
#include "Server.h"
#include <iostream>
#include <thread>

auto Server::Run(uint16 port) -> bool
{
	if (mSocket.Init() == false)
		return false;

	if (mIocp.Init() == false)
		return false;

	if (init(port) == false)
		return false;

	std::cout << "server on" << std::endl;
	DWORD recvBytes;
	DWORD cbTransfereed;
	while (true)
	{
		TcpStream client;
		client.Init();
		if (false == TcpStream::LpFnAcceptEx(mSocket.GetServerSocketPtr()->GetSocketInfoPtr()->socket,
			client.GetSocket(),
			client.GetBuffer(),
			0,
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&recvBytes,
			reinterpret_cast<LPOVERLAPPED>(mSocket.GetOverlappedPtr())))
		{
			const int32 error = WSAGetLastError();
			if (error != WSA_IO_PENDING)
			{
				// 채워 넣기
				std::cout << error << std::endl;
			}
			else
			{
				std::cout << error << std::endl;
			}
		}
		else
		{
			mSocket.SetSocket(SO_UPDATE_ACCEPT_CONTEXT);
		}
		int addrLen = sizeof(SOCKADDR_IN);
		bool retval = GetQueuedCompletionStatus(mIocp.GetHandle(), &cbTransfereed, &client.GetSocketInfoPtr()->socket, (LPOVERLAPPED*)&client.GetSocketInfoPtr()->overlapped, INFINITE);
		getpeername(client.GetSocketInfoPtr()->socket, (SOCKADDR*)client.GetAddrPtr(), &addrLen);
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client.GetAddrPtr()->sin_addr, addr, sizeof(addr));
		if (client.GetIOEvent() == IOEvent::ACCEPT)
		{
			std::cout << "IP: " << addr << "Port: " << ntohs(client.GetAddrPtr()->sin_port) << std::endl;
			std::cout << static_cast<int>(reinterpret_cast<OVERLAPPEDEX*>(client.GetOverlappedPtr())->event) << std::endl;
		}
		else
		{
			char str[2048];
			ZeroMemory(str, 2048);
			memcpy(str, client.GetBuffer()->buf, client.GetBuffer()->len);
			std::cout << str << std::endl;
		}
		
		
	}
	

	return true;
}

auto Server::Run(std::string addr, uint16 port) -> bool
{
	// Error 따로 정의 해서 내보내기
	if (init(addr, port) == false)
		return false;

	if (mIocp.GetHandle() == NULL)
		return false;

	if (mIocp.Register(*mSocket.GetServerSocketPtr()) == NULL)
		return false;

	return true;
}

auto Server::Close() -> bool
{
	WSACleanup();
	return false;
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

	HANDLE h = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	mIocp.SetHandle(h);

	if (mIocp.GetHandle() == NULL)
		return false;

	if (mIocp.Register(*mSocket.GetServerSocketPtr()) == NULL)
		return false;

	return true;
}

auto Server::init(std::string addr, uint16 port) -> bool
{
	mSocket.Bind(addr, port);

	HANDLE h = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	mIocp.SetHandle(h);

	if (mIocp.GetHandle() == NULL)
		return false;

	if (mIocp.Register(*mSocket.GetServerSocketPtr()) == NULL)
		return false;

	return true;
}
