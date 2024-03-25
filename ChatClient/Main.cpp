#include "pch.h"
#include "AsyncStream.h"
#include "Memory.h"
#include "NetworkEndpoint.h"
#include "Client.h"

int main()
{
	// Example
	::Sleep(100);
	NetworkInit();
	Client client;
	client.Connect("127.0.0.1", 8000);
	char str[]{ "Hello, Server!\n" };
	client.Dispatch();
	client.Send(1, sizeof(str), str);
	while (true)
	{
		client.Dispatch();
	}

	getchar();

	return 0;
}