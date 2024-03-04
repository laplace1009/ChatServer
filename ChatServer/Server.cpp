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

		/*TcpStream client = mSocket.Accept();
		std::cout << "socket: " << client.GetSocketInfoPtr()->socket << std::endl;
		::Sleep(1000);

		if (GetQueuedCompletionStatus(mIocp.GetHandlePtr(), &mSocket.GetServerSocketPtr()->GetSocketInfoPtr()->recvBytes,
			&mSocket.GetServerSocketPtr()->GetSocketInfoPtr()->socket,
			reinterpret_cast<LPOVERLAPPED*>(&mSocket.GetServerSocketPtr()->GetSocketInfoPtr()->overlapped), INFINITE))
		{

		}*/

	for (int32 i = 0; i < 5; ++i)
	{
		GThreads->Launch([=]()
			{
				while (true)
				{
					TcpStream client = mSocket.Accept();
					
				}
			});
	}

	

	return true;
}

auto Server::Run(std::string_view addr, uint16 port) -> bool
{
	// Error 따로 정의 해서 내보내기
	if (init(addr, port) == false)
		return false;

	if (*mIocp.GetHandlePtr() == NULL)
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
	if (mSocket.Init() == false)
		return false;
	
	mSocket.BindAny(port);

	*mIocp.GetHandlePtr() = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (*mIocp.GetHandlePtr() == NULL)
		return false;

	if (mIocp.Register(*mSocket.GetServerSocketPtr()) == NULL)
		return false;

	return true;
}

auto Server::init(std::string_view addr, uint16 port) -> bool
{
	if (mSocket.Init())
		return false;

	mSocket.Bind(addr, port);

	*mIocp.GetHandlePtr() = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (*mIocp.GetHandlePtr() == NULL)
		return false;

	if (mIocp.Register(*mSocket.GetServerSocketPtr()) == NULL)
		return false;

	return true;
}
