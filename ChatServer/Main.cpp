#include "pch.h"
#include <iostream>
#include "Server.h"

using namespace std;

int main(void)
{
	Server server;
	server.Start(8000);

	/*for (int i = 0; i < 1; ++i)
	{
		GThreads->Launch([&server]()
			{
				server.IOAction();
			});
	}

	GThreads->Join();
	server.Close();*/
	while (true)
	{
		server.Accept();
		::Sleep(500);
	}
	getchar();
	
	return 0;
}