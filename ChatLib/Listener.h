#pragma once
#include "Network.h"

class Listener : public Network
{
public:
	virtual ~Listener() = default;

public:									
	virtual bool Accept() = 0;
};

