#pragma once
#include "Types.h"
#include "IOCP.h"
#include "Error.h"
#include "ReadWriteLock.h"
#include "ReadLockGuard.h"
#include "WriteLockGuard.h"
#include "AsyncStream.h"
#include "AsyncListener.h"
#include "NetworkEndpoint.h"
#include <vector>
#include <string>

class alignas(64) Server : public IOCP
{
	enum : uint16
	{
		MAX_MEMEBERS = 100,
		ACCEPT_COUNT = 16,
	};

public:
	Server();
	~Server() noexcept;

public:
	NODISCARD Error Register(LPAsyncEndpoint socket)	override;
	NODISCARD Error Dispatch()							override;

public:
	NODISCARD auto Run(uint16)							-> Error;
	NODISCARD auto Run(std::string addr, uint16 port)	-> Error;

public:
	auto GetHandle() -> HANDLE;
	auto GetSocket() -> SOCKET;

public:
	auto Send()								-> Error;
	auto SetRecv(LPAsyncEndpoint client)	-> Error;

private:
	auto accept()									-> void;
	auto acceptRegister()							-> void;
	auto acceptRegister(LPAsyncEndpoint client)		-> void;
	auto setMsg(WSABUF* dst, LPAsyncEndpoint src)	-> void;
	auto doIOAction(LPAsyncEndpoint client)			-> void;
	auto setEventAccept(LPAsyncEndpoint client)		-> void;
	auto setEventRecv(LPAsyncEndpoint client)		-> void;
	auto setEventSend(LPAsyncEndpoint client)		-> void;
	auto setEventDisconnect(LPAsyncEndpoint client) -> void;
	auto afterIOAcceptEvent(LPAsyncEndpoint client) -> void;
	auto afterIORecvEvent(LPAsyncEndpoint client)	-> void;
	auto afterIOSendEvent(LPAsyncEndpoint client)	-> void;
	auto afterIODisconnect(LPAsyncEndpoint client)	-> void;

private:
	AsyncListener										mListener;
	HANDLE												mHandle;
	ReadWriteLock										mLock;
	std::queue<WSABUF*>									mSendBuffs;
	std::vector<LPAsyncEndpoint>						mClients;
	
private:
	static Atomic<uint32>								mId;
};

