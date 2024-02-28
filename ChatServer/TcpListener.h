#pragma once
#include <string_view>
#include <optional>
#include "TcpStream.h"

class TcpListener
{
public:
	auto BindAny(uint16 port) -> void;
	auto Bind(std::string_view addr, uint16 port) -> void;
	auto Accept() -> std::optional<TcpStream>;
	auto Recv(OUT TcpStream* client) -> int;
	auto Send(TcpStream* client, int retVal) -> int;
	auto SwitchSyncAsync(u_long swt) -> int;
	auto GetServerSocketPtr() -> TcpStream*;

private:
	TcpStream mServer;
};

