#pragma once
#include "AsyncStream.h"

class IOCP
{
	virtual bool Register(AsyncStream*) = 0;
	virtual bool Dispatch() = 0;
};

