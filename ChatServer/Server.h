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
	NODISCARD bool Register(AsyncStream* stream) override;
	NODISCARD bool Dispatch() override;
	bool IOAccept(AsyncStream* client) override;
	bool IORecv(AsyncStream* client) override;
	bool IOSend(AsyncStream* client) override;
	bool IODisconnect(AsyncStream* client) override;

public:
	NODISCARD auto Run(uint16) -> bool;
	NODISCARD auto Run(std::string addr, uint16 port) -> bool;

public:
	auto GetHandle() -> HANDLE;

private:
	auto accept() -> void;
	auto acceptRegister(AsyncStream* client) -> void;

private:
	AsyncListener mListener;
	HANDLE mHandle;
	std::vector<AsyncStream*> mClients;
};

