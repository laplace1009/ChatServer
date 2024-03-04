#pragma once
#include <array>
#include <atomic>
#include "Types.h"
#include "TcpStream.h"

struct User
{
	static std::atomic<uint32> id;
	TcpStream client;
};

class Room
{
public:
	auto Join(TcpStream stream) -> bool;
	auto Out() -> void;

private:
	auto existSocket(SOCKET socket) -> bool;

private:
	std::array<User, 100> mUsers;
};

