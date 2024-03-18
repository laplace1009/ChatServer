#include "pch.h"
#include "Client.h"

Client::Client() : mId{0}
{
	mClient.Init();
	mClient.GetOverlappedPtr()->SetOwner(this);
	mClient.SetSocket(AsyncStream::CreateSocket());
	ASSERT_CRASH(mClient.ConstGetSocket() == INVALID_SOCKET);
}

auto Client::Init(uint16 port) -> void
{
	mClient.BindAny(port);
}

auto Client::Init(std::string addr, uint16 port) -> void
{
	mClient.Bind(addr, port);
}