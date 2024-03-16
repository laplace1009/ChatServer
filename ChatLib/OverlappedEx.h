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
	auto GetOwner() -> AsyncStream*;
	
private:
	IOEvent mEvent;
	AsyncStream* owner;
};

