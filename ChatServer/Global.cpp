#include "pch.h"
#include "Global.h"
#include "Memory.h"
#include "MultiSizeMemory.h"

MultiSizeMemory* GMemory = nullptr;

class Global
{
public:
	Global()
	{
		GMemory = new MultiSizeMemory();
	}

	~Global()
	{
		delete GMemory;
	}

} GGlobal;

