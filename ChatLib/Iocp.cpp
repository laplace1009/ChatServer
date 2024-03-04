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

auto Iocp::GetHandlePtr() -> HANDLE*
{
	return &mHandle;
}

auto Iocp::Register(TcpStream& stream) -> bool
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(stream.GetSocketInfoPtr()->socket), mHandle, stream.GetSocketInfoPtr()->socket, 0))
		return false;

	return true;
}
