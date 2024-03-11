#pragma once

class Listener
{
public:
	virtual bool Init() = 0;
	virtual bool BindAny(uint16 port) = 0;
	virtual bool Bind(std::string addr, uint16 port) = 0;
	virtual bool Accept(SOCKET socket, SOCKADDR_IN sockAddr) = 0;
	virtual bool Recv() = 0;
	virtual bool Send() = 0;
};

