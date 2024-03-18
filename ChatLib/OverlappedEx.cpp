#include "pch.h"
#include "OverlappedEx.h"

OverlappedEx::OverlappedEx()
{
	ZeroMemory(this, sizeof(OVERLAPPED));
	mEvent = IOEvent::ACCEPT;
	mOwner = nullptr;
}

OverlappedEx::~OverlappedEx()
{
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

auto OverlappedEx::GetOwner() -> void*
{
	return mOwner;
}

auto OverlappedEx::SetOwner(void* owner) -> void
{
	mOwner = owner;
}
