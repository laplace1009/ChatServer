#pragma once
#include "AsyncStream.h"

class IOCP
{
	virtual bool Register(AsyncStream* stream) = 0;
	virtual bool Dispatch() = 0;
};

