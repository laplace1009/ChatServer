#include "pch.h"
#include "TcpListener.h"

auto TcpListener::Init() -> bool
{
	if (WSAStartup(MAKEWORD(2, 2), &mWsaData) != 0)
		return false;

	return mServer.Init();
}

auto TcpListener::BindAny(uint16 port) -> void
{
	mServer.GetSocketInfoPtr()->addr.sin_family = AF_INET;
	mServer.GetSocketInfoPtr()->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	mServer.GetSocketInfoPtr()->addr.sin_port = htons(port);
	if (SOCKET_ERROR == bind(mServer.GetSocketInfoPtr()->socket, reinterpret_cast<SOCKADDR*>(&mServer.GetSocketInfoPtr()->addr), sizeof(mServer.GetSocketInfoPtr()->addr)))
		return;

	if (SOCKET_ERROR == listen(mServer.GetSocketInfoPtr()->socket, SOMAXCONN))
		return;
}

auto TcpListener::Bind(std::string_view addr, uint16 port) -> void
{
	mServer.GetSocketInfoPtr()->addr.sin_family = AF_INET;
	mServer.GetSocketInfoPtr()->addr.sin_port = htons(port);
	int isSuccess = inet_pton(AF_INET, addr.data(), &mServer.GetSocketInfoPtr()->addr);
	// wrong string address
	if (0 == isSuccess)
	{
		return;
	}
	// internal error
	if (-1 == isSuccess)
	{
		return;
	}
	if (SOCKET_ERROR == bind(mServer.GetSocketInfoPtr()->socket, reinterpret_cast<SOCKADDR*>(&mServer.GetSocketInfoPtr()->addr), sizeof(mServer.GetSocketInfoPtr()->addr)))
		return;
	if (SOCKET_ERROR == listen(mServer.GetSocketInfoPtr()->socket, SOMAXCONN))
		return;
}

auto TcpListener::Accept() -> TcpStream
{
	TcpStream client;
	int addrLen = sizeof(client.GetSocketInfoPtr()->addr);
	client.GetSocketInfoPtr()->socket = accept(mServer.GetSocketInfoPtr()->socket, reinterpret_cast<SOCKADDR*>(&client.GetSocketInfoPtr()->addr), &addrLen);

	return client;
}

auto TcpListener::AcceptEx() -> TcpStream
{
	TcpStream client;
	DWORD bytesReceived = 0;

	if (false == ::AcceptEx(mServer.GetSocketInfoPtr()->socket, client.GetSocketInfoPtr()->socket,
		client.GetSocketInfoPtr()->buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived,
		static_cast<LPOVERLAPPED>(&client.GetSocketInfoPtr()->overlapped)))
	{
		const int32 error = ::WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			AcceptEx();
		}
	}


	return client;
}

auto TcpListener::Recv(OUT TcpStream* client) -> int
{
	return recv(client->GetSocketInfoPtr()->socket, reinterpret_cast<char*>(client->GetSocketInfoPtr()->buf), sizeof(client->GetSocketInfoPtr()->addr), 0);
}

auto TcpListener::Send(TcpStream* client, int retVal) -> int
{
	return send(client->GetSocketInfoPtr()->socket, reinterpret_cast<char*>(client->GetSocketInfoPtr()->buf), retVal, 0);
}

auto TcpListener::SwitchSyncAsync(u_long swt) -> int
{
	return ioctlsocket(mServer.GetSocketInfoPtr()->socket, FIONBIO, &swt);
}

auto TcpListener::GetServerSocketPtr() -> TcpStream*
{
	return &mServer;
}

