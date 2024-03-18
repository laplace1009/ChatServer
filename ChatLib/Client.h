#pragma once
#include "Types.h"
#include "AsyncStream.h"

class Client
{
	
public:
	Client();

public:
	auto Init(uint16 port) -> void;
	auto Init(std::string addr, uint16 port) -> void;

private:
	uint32 mId;
	AsyncStream mClient;
};

