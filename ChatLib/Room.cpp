#include "pch.h"
#include "Room.h"

std::atomic<uint32> User::id = 0;

auto Room::existSocket(SOCKET socket) -> bool
{
	for (const auto& user : mUsers)
	{
		if (user.client.GetSocketInfoPtr()->socket == socket)
			return true;
	}
	return false;
}

auto Room::Join(TcpStream stream) -> bool
{
	SOCKET newSocket = stream.GetSocketInfoPtr()->socket;
	if (existSocket(newSocket))
		return false;

	

	return true;
}

auto Room::Out() -> void
{

}


