#pragma once

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
	
private:
	IOEvent mEvent;
};

