#pragma once
#include "Types.h"
#include "TcpStream.h"
#include "TcpListener.h"
#include "Iocp.h"
#include <vector>
#include <string_view>

class Server
{
	enum
	{
		MAX_MEMEBERS = 100,
	};

public:
	auto Run(uint16 port) -> bool;
	auto Run(std::string_view addr, uint16 port) -> bool;
	auto Close() -> bool;

	auto Join(TcpStream&& stream) -> bool;

private:
	auto init(uint16 port) -> bool;
	auto init(std::string_view addr, uint16 port) -> bool;

private:
	TcpListener mSocket;
	Iocp mIocp;
	Vector<TcpStream> mClients;
};

