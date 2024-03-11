#include "pch.h"
#include "AsyncListener.h"

bool AsyncListener::BindAny(uint16 port)
{
	
	return false;
}

bool AsyncListener::Bind(std::string addr, uint16 port)
{
	return false;
}

bool AsyncListener::Accept(SOCKET socket, SOCKADDR_IN sockAddr)
{
	return false;
}

bool AsyncListener::Recv()
{
	return false;
}

bool AsyncListener::Send()
{
	return false;
}
