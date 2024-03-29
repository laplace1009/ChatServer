#include "pch.h"
#include "Server.h"
#include "AsyncListener.h"

using namespace std;

auto DoJobWorker(Server& server) -> void;

int main(void)
{
	SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);
	DWORD threadCounts = systeminfo.dwNumberOfProcessors;
	
	if (NetworkInit() == Error::NET_WSA_INIT_ERROR)
		return 1;

	Server server;
	Error serverRunError = server.Run(8000);
	if (serverRunError == Error::NET_BIND_ERROR || serverRunError == Error::NET_LISTEN_ERROR)
	{
		std::cerr << "Server Run Error\n";
	}
	for (size_t i = 0; i < 1; ++i)
	{
		GThreads->Launch([&server]()
			{
				DoJobWorker(server);
			});
	}

	DoJobWorker(server);

	GThreads->Join();

	return 0;
}

auto DoJobWorker(Server& server) -> void
{
	while (true)
	{
		if (server.Dispatch() == Error::IOCP_DISPATCH_ERROR)
		{
			std::cerr << "IOCP Error\n";
		}
	}
}