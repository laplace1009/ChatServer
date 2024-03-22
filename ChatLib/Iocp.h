#pragma once
#include "NetworkEndpoint.h"

class IOCP
{
	virtual bool Register(LPAsyncEndpoint socket) = 0;
	virtual bool Dispatch()						  = 0;
};

