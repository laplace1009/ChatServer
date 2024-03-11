#pragma once
#include "Types.h"
#include "AsyncStream.h"
#include "Listener.h"

class AsyncListener : public Listener
{
public:
	bool BindAny(uint16 port) override;
	bool Bind(std::string addr, uint16 port) override;
	bool Accept(SOCKET socket, SOCKADDR_IN sockAddr) override;
	bool Recv() override;
	bool Send() override;

private:
	AsyncStream mListener;
};

