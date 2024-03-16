#pragma once
#include "Stream.h"

class IOCP
{
	virtual bool Register(Stream* stream) = 0;
	virtual bool Dispatch() = 0;
};

