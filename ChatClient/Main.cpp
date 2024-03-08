#include "pch.h"
#include "Iocp.h"
#include "TcpStream.h"
#include "Memory.h"

int main()
{
	using namespace std;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	string str{ "Hello, World!\n" };
	//std::this_thread::sleep_for(500ms);
	TcpStream client;
	if (client.Init() == false)
		return 1;
	client.Bind();
	if (client.Connect("127.0.0.1", 8000))
	{
		if (client.SetSocketOpt(SO_UPDATE_CONNECT_CONTEXT) == false)
		{
			client.SetSocketOpt(SO_UPDATE_CONNECT_CONTEXT);
		}
		else
		{
			cout << "SetSockOpt success\n";
		}
	}

	memcpy(client.GetBuffer()->buf, str.data(), str.length());
	client.GetBuffer()->len = str.length();
	
	if (TcpStream::Send(client))
	{
		cout << "Send Success\n";
	}
	else
	{
		cout << "Send failure\n";
	}

	/*if (client.Init())
	{
		cout << "init ok\n";
		cout << static_cast<int>(reinterpret_cast<OVERLAPPEDEX*>(client.GetOverlappedPtr())->event) << endl;
	}
	else
		cout << "init error\n";
	client.Bind();
	
	cout << TcpStream::LpFnConnectEx << endl;
	if (client.Connect("127.0.0.1", 8000))
		cout << "Connect\n";
	else
	{
		cout << WSAGetLastError() << endl;
	}
	client.Send(client);
	getchar();*/
	getchar();

	return 0;
}