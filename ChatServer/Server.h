#pragma once
#include "Types.h"
#include "IOCP.h"
#include "ReadWriteLock.h"
#include "ReadLockGuard.h"
#include "WriteLockGuard.h"
#include "AsyncStream.h"
#include "AsyncListener.h"
#include "NetworkEndpoint.h"
#include <vector>
#include <string>

class Server : public IOCP
{
	enum
	{
		MAX_MEMEBERS = 100,
	};

public:
	Server();
	~Server() noexcept;

public:
	NODISCARD bool Register(LPAsyncEndpoint socket)	override;
	NODISCARD bool Dispatch()						override;

public:
	NODISCARD auto Run(uint16)							-> bool;
	NODISCARD auto Run(std::string addr, uint16 port)	-> bool;

public:
	auto GetHandle() -> HANDLE;
	auto GetSocket() -> SOCKET;

private:
	auto accept()								-> void;
	auto acceptRegister(LPAsyncEndpoint client)	-> void;
	auto setMsg(CHAR* msg, size_t size)			-> bool;
	auto IOConnect(LPAsyncEndpoint client)		-> void;
	auto IOAccept(LPAsyncEndpoint client)		-> void;
	auto IORecv(LPAsyncEndpoint client)			-> void;
	auto IOSend(CHAR* msg, size_t size)			-> void;
	auto IODisconnect(LPAsyncEndpoint client)	-> void;
	auto Log(const char* msg)					-> void;

private:
	AsyncListener					mListener;
	HANDLE							mHandle;
	std::vector<LPAsyncEndpoint>	mClients;
	WSABUF							mRecvBuf;
	static Atomic<uint32>			mId;
};

