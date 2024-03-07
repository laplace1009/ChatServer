#include "pch.h"
#include "Iocp.h"
#include "TcpStream.h"
#include "Memory.h"

int main()
{
	using namespace std;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	std::this_thread::sleep_for(500ms);
	TcpStream client;
	if (client.Init())
	{
		cout << "init ok\n";
	}
	else
		cout << "init error\n";
	cout << TcpStream::LpFnConnectEx << endl;
	if (client.Connect("127.0.0.1", 8000))
		cout << "Connect\n";
	else
	{
		cout << WSAGetLastError() << endl;
	}
	getchar();

	return 0;
}