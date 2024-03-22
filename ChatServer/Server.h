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

class alignas(64) Server : public IOCP
{
	enum : uint16
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

public:
	auto Send() -> bool;
	auto SetRecv(LPAsyncEndpoint client) -> bool;

private:
	auto accept()									-> void;
	auto acceptRegister(LPAsyncEndpoint client)		-> void;
	auto setMsg(WSABUF* dst, LPAsyncEndpoint src)	-> void;
	auto IOAccept(LPAsyncEndpoint client)			-> void;
	auto IORecv(LPAsyncEndpoint client)				-> void;
	auto IOSend(LPAsyncEndpoint client)				-> void;
	auto IODisconnect(LPAsyncEndpoint client)		-> void;
	auto Log(const char* msg)						-> void;
	//auto setEventRecv() -> void;
	auto setEventAccept(LPAsyncEndpoint client) -> void;
	//auto setEventDisconnect() -> void;
	//auto setSend() -> void;

private:
	AsyncListener					mListener;
	HANDLE							mHandle;
	std::vector<LPAsyncEndpoint>	mClients;
	std::queue<WSABUF*>				mSendBuffs;
	ReadWriteLock					mLock;
	static Atomic<uint32>			mId;
};

