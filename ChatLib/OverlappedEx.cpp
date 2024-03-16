#include "pch.h"
#include "OverlappedEx.h"

OverlappedEx::OverlappedEx()
{
	ZeroMemory(this, sizeof(OVERLAPPED));
	mEvent = IOEvent::ACCEPT;
	mOwner = nullptr;
}

auto OverlappedEx::SetIOEVent(IOEvent event) -> void
{
	mEvent = event;
}

auto OverlappedEx::GetIOEvent() -> IOEvent
{
	return mEvent;
}

auto OverlappedEx::GetOwner() -> AsyncStream*
{
	return mOwner;
}

auto OverlappedEx::SetOwner(AsyncStream* owner) -> void
{
	mOwner = owner;
}
