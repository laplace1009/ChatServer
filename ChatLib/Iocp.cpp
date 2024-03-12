#include "pch.h"
#include "Iocp.h"

Iocp::~Iocp() noexcept
{
	CloseHandle(mHandle);
}

auto Iocp::Init() -> bool
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (mHandle == NULL)
		return false;

	return true;
}

auto Iocp::GetHandle() -> HANDLE
{
	return mHandle;
}

auto Iocp::SetHandle(HANDLE h) -> void
{
	mHandle = h;
}

auto Iocp::Register(AsyncStream& stream) -> bool
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(stream.ConstGetSocket()), mHandle, 0, 0))
		return false;

	return true;
}

auto Iocp::Dispatch(uint32 timeout) -> bool
{
	DWORD bytes{ 0 };
	AsyncStream* client = xnew<AsyncStream>();
	if (GetQueuedCompletionStatus(mHandle, OUT & bytes, OUT reinterpret_cast<PULONG_PTR>(client->ConstGetSocket()), OUT reinterpret_cast<LPOVERLAPPED*>(client->GetLPOveralppedPtr()), timeout))
	{
		std::cout << "Client Connect!\n";
	}

	return true;
}

