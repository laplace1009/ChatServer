#include "pch.h"
#include <WinSock2.h>

enum class IOEvent
{
	ACCEPT,
	RECV,
	SEND,
	DISCONNECT,
};

int main()
{
	using namespace std;

	cout << "Hello, World\n" << endl;
	
	return 0;
}