#pragma once
#include "Types.h"
#include "Iocp.h"
#include "ReadWriteLock.h"
#include "ReadLockGuard.h"
#include "WriteLockGuard.h"
#include "AsyncStream.h"
#include "AsyncListener.h"
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
	NODISCARD bool Register(AsyncStream* stream)	override;
	NODISCARD bool Dispatch()						override;

public:
	NODISCARD auto Run(uint16)							-> bool;
	NODISCARD auto Run(std::string addr, uint16 port)	-> bool;

public:
	auto GetHandle() -> HANDLE;
	auto GetSocket() -> SOCKET;

private:
	auto accept()												-> void;
	auto acceptRegister(AsyncStream* client)					-> void;
	auto setMsg(CHAR* msg, size_t size)							-> bool;
	auto IOConnect(AsyncStream* client)							-> void;
	auto IOAccept(AsyncStream* client)							-> void;
	auto IORecv(AsyncStream* client)							-> void;
	auto IOSend(AsyncStream* client, CHAR* msg, size_t size)	-> void;
	auto IODisconnect(AsyncStream* client)						-> void;
	auto Log(const char* msg)									-> void;

private:
	AsyncListener mListener;
	HANDLE mHandle;
	std::vector<AsyncStream*> mClients;
	static Atomic<uint32> mId;
};

