#include "pch.h"
#include <WinSock2.h>
#include "Iocp.h"
#include "TcpStream.h"
#include <thread>

int main()
{
	using namespace std;
	std::this_thread::sleep_for(1000ms);
	IOEvent event = IOEvent::ACCEPT;
	TcpStream client;
	client.Init();
	client.Connect("127.0.0.1", 8000);
	return 0;
}