#pragma once
#include <string_view>
#include "TcpStream.h"

class TcpListener
{
public:
	auto Init() -> bool;
	auto BindAny(uint16 port) -> void;
	auto Bind(std::string_view addr, uint16 port) -> void;
	auto Accept() -> TcpStream;
	auto Recv(OUT TcpStream* client) -> int;
	auto Send(TcpStream* client, int retVal) -> int;
	auto SwitchSyncAsync(u_long swt) -> int;
	auto GetServerSocketPtr() -> TcpStream*;

private:
	WSADATA mWsaData;
	TcpStream mServer;
};

