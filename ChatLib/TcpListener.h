#pragma once
#include <string_view>
#include "TcpStream.h"

class TcpListener
{
public:
	auto Init() -> bool;
	auto BindAny(uint16 port) -> void;
	auto Bind(std::string addr, uint16 port) -> void;
	auto Accept() -> TcpStream;
	auto Recv(OUT TcpStream& client) -> int;
	auto Send(TcpStream& client) -> int;
	auto SwitchSyncAsync(u_long swt) -> int;
	auto GetStreamPtr() -> TcpStream*;

public:
	auto GetSocket() const -> const SOCKET;
	auto GetSocket() -> SOCKET;
	auto SetSocket(SOCKET socket) -> void;
	auto GetAddrPtr() -> SOCKADDR_IN*;
	auto GetBuffer() -> WSABUF*;
	auto GetRecvBytes() const -> const DWORD;
	auto SetRecvBytes(DWORD size) -> void;
	auto GetSendBytes() const -> const DWORD;
	auto SetSendBytes(DWORD size) -> void;
	auto GetOverlappedPtr() -> LPOVERLAPPED;

private:
	TcpStream mStream;
};

