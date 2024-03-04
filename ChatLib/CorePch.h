#pragma once

#include "Types.h"
#include "Macro.h"
#include "Global.h"
#include "Container.h"
#include "TLS.h"
#include "Thread.h"

#include <iostream>
#include <string_view>
#include <vector>
#include <chrono>
#include <thread>

#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")