#pragma once

#include "Types.h"
#include "Macro.h"
#include "Global.h"
#include "Container.h"
#include "TLS.h"
#include "Thread.h"
#include "ReadWriteLock.h"
#include "ReadLockGuard.h"
#include "WriteLockGuard.h"

#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <type_traits>
#include <queue>
#include <functional>

#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")