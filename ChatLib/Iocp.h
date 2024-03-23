#pragma once
#include "NetworkEndpoint.h"

class IOCP
{
	virtual Error Register(LPAsyncEndpoint socket)	= 0;
	virtual Error Dispatch()						= 0;
};

