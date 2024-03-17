#pragma once
#include "AsyncStream.h"

class IOCP
{
	virtual bool Register(AsyncStream*) = 0;
	virtual bool Dispatch() = 0;
	virtual bool IOAccept(AsyncStream*) = 0;
	virtual bool IORecv(AsyncStream*) = 0;
	virtual bool IOSend(AsyncStream*) = 0;
	virtual bool IODisconnect(AsyncStream*) = 0;
};

