#pragma once
#include "AsyncStream.h"

class AsyncStream;

enum class IOEvent
{
	ACCEPT,
	RECV,
	SEND,
	DISCONNECT,
};

class OverlappedEx : public WSAOVERLAPPED
{

public:
	OverlappedEx();

public:
	auto SetIOEVent(IOEvent event) -> void;
	auto GetIOEvent() -> IOEvent;
	auto GetOwner() -> AsyncStream*;
	auto SetOwner(AsyncStream* owner) -> void;

private:
	IOEvent mEvent;
	AsyncStream* mOwner;
};

