#include "pch.h"
#include "OverlappedEx.h"

OverlappedEx::OverlappedEx()
{
	ZeroMemory(this, sizeof(OVERLAPPED));
	mEvent = IOEvent::ACCEPT;
}

auto OverlappedEx::SetIOEVent(IOEvent event) -> void
{
	mEvent = event;
}
