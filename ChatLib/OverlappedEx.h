#pragma once

enum class IOEvent
{
	CONNECT,
	ACCEPT,
	RECV,
	SEND,
	DISCONNECT,
};

class OverlappedEx : public WSAOVERLAPPED
{
public:
	OverlappedEx();
	~OverlappedEx();

public:
	auto SetIOEVent(IOEvent event)	-> void;
	auto GetIOEvent()				-> IOEvent;
	auto GetOwner()					-> void*;
	auto SetOwner(void* owner)		-> void;

private:
	IOEvent mEvent;
	void* mOwner;
};

