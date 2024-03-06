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
	TcpStream stream;
	stream.Init();
	SOCKET client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	DWORD cbTransfereed;
	char TestBuffer[1024];
	while (true)
	{
		
		if (TcpStream::LpFnAcceptEx(mSocket.GetServerSocketPtr()->GetSocketInfoPtr()->socket,
			stream.GetSocketInfoPtr()->socket,
			stream.GetSocketInfoPtr()->buf,
			0,
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			&cbTransfereed,
			reinterpret_cast<LPOVERLAPPED>(&mSocket.GetServerSocketPtr()->GetSocketInfoPtr()->overlapped)))
		{

		}
		else
		{
			std::cout << WSAGetLastError() << std::endl;
			Sleep(500);
			continue;
		}
		/*int addrLen = sizeof(SOCKADDR_IN);
		int retval = GetQueuedCompletionStatus(mIocp.GetHandle(), &cbTransfereed, (PULONG_PTR)&client.GetSocketInfoPtr()->socket, (LPOVERLAPPED*)&client.GetSocketInfoPtr()->overlapped, INFINITE);
		std::cout << retval << std::endl;
		getpeername(client.GetSocketInfoPtr()->socket, (SOCKADDR*)&client.GetSocketInfoPtr()->addr, &addrLen);
		inet_ntop(AF_INET, &client.GetSocketInfoPtr()->addr.sin_addr, client.GetSocketInfoPtr()->buf, 4096);
		if (retval == 0 || cbTransfereed == 0)
		{
			std::cout << "[TCP 서버] 클라이언트 종료: IP 주소 = " << client.GetSocketInfoPtr()->buf << ", 포트 번호 = " << ntohs(client.GetSocketInfoPtr()->addr.sin_port) << std::endl;
			break;
		}
		::Sleep(100);*/
	}
	

	return true;
}

auto Server::Run(std::string_view addr, uint16 port) -> bool
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

auto Server::init(std::string_view addr, uint16 port) -> bool
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
