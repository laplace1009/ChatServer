#include "pch.h"
#include "Iocp.h"
#include "AsyncStream.h"
#include "Memory.h"

int main()
{
	using namespace std;
	HANDLE h = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	AsyncStream client;
	client.Init();
	client.SetSocket(AsyncStream::CreateSocket());
	::CreateIoCompletionPort((HANDLE)client.ConstGetSocket(), h, 0, 0);
	client.BindAny(0);
	client.SetAddr("127.0.0.1", 8000);
	if (client.Connect() == false)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			cout << error << endl;
		}
	}
	/*WSAOVERLAPPED* overlapped = nullptr;
	DWORD transferredbytes{ 0 };
	bool ret = GetQueuedCompletionStatus(iocp.GetHandle(), &transferredbytes, 0, &overlapped, INFINITE);
	if (ret)
	{
		cout << "Connected Server!\n";
	}*/
	//AsyncStream stream;
	//stream.Init();

	getchar();

	return 0;
}