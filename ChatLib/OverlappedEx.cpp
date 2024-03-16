#include "pch.h"
#include "OverlappedEx.h"

OverlappedEx::OverlappedEx()
{
	ZeroMemory(this, sizeof(OVERLAPPED));
	mEvent = IOEvent::ACCEPT;
	owner = nullptr;
}

auto OverlappedEx::SetIOEVent(IOEvent event) -> void
{
	mEvent = event;
}

auto OverlappedEx::GetOwner() -> AsyncStream*
{
	return owner;
}