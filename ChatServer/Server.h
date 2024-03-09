#pragma once
#include "Types.h"
#include "TcpStream.h"
#include "TcpListener.h"
#include "Iocp.h"
#include "ReadWriteLock.h"
#include "ReadLockGuard.h"
#include "WriteLockGuard.h"
#include <vector>
#include <string>

class Server
{
	enum
	{
		MAX_MEMEBERS = 100,
	};

public:
	auto Start(uint16 port) -> bool;
	auto Start(std::string addr, uint16 port) -> bool;
	auto Close() -> bool;
	
	auto IOAction() -> bool;
	auto Accept() -> void;
	auto Join(TcpStream&& stream) -> bool;

private:
	auto init(uint16 port) -> bool;
	auto init(std::string addr, uint16 port) -> bool;

private:
	ReadWriteLock mLock;
	TcpListener mSocket;
	Iocp mIocp;
	std::vector<TcpStream> mClients;
};

