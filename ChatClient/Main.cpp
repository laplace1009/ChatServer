#include "pch.h"
#include "AsyncStream.h"
#include "Memory.h"
#include "NetworkEndpoint.h"
#include "Client.h"

void DoJobWorkerThread(Client&);
void DoJobMainThread(Client&);

int main()
{
	// Example
	::Sleep(1000);
	NetworkInit();
	Client client;
	client.Connect("127.0.0.1", 8000);
	std::jthread t([&client]()
		{
			DoJobWorkerThread(client);
		});

	DoJobMainThread(client);

	return 0;
}

void DoJobWorkerThread(Client& client)
{
	while (true)
	{
		client.Dispatch();
	}
}

void DoJobMainThread(Client& client)
{
	while (true)
	{
		String msg;
		std::getline(std::cin, msg);
		client.Send(0, msg.length(), msg.data());
	}
}