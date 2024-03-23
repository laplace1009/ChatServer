#include "pch.h"
#include "Global.h"
#include "Memory.h"
#include "MultiSizeMemory.h"
#include "Thread.h"

MultiSizeMemory* GMemory = nullptr;
Thread* GThreads = nullptr;

class Global
{
public:
	Global()
	{
		GMemory = new MultiSizeMemory();
		GThreads = new Thread();
	}
	
	~Global()
	{
		delete GMemory;
		delete GThreads;
	}

} GGlobal;

