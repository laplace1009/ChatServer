#include "pch.h"
#include "Iocp.h"
#include "TcpStream.h"
#include "Memory.h"

int main()
{
	using namespace std;
	/*WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	std::this_thread::sleep_for(1000ms);
	TcpStream client;
	client.Init();
	client.SetAddr("127.0.0.1", 8000);
	cout << "client start" << endl;
	while (1)
	{
		if (TcpStream::LpFnConnectEx(client.GetSocketInfoPtr()->socket,
			reinterpret_cast<SOCKADDR*>(&client.GetSocketInfoPtr()->addr),
			sizeof(client.GetSocketInfoPtr()->addr),
			nullptr,
			0,
			&client.GetSocketInfoPtr()->recvBytes,
			reinterpret_cast<LPOVERLAPPED>(&client.GetSocketInfoPtr()->overlapped)))
		{
			cout << "server connect success\n";
		}
		else
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
			{
				std::cout << "io_pending\n";
			}
			else
			{
				cout << WSAGetLastError() << endl;
			}
		}
		std::this_thread::sleep_for(500ms);
	}
	WSACleanup();*/


	return 0;
}