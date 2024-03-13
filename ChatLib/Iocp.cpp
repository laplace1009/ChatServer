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
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(stream.ConstGetSocket()), mHandle, reinterpret_cast<ULONG_PTR>(&stream), 0))
		return false;

	return true;
}

auto Iocp::Dispatch(uint32 timeout) -> bool
{
	DWORD bytes{ 0 };
	AsyncStream* client = nullptr;
	OverlappedEx* overlapped = nullptr;
	ULONG_PTR completionKey;
	if (GetQueuedCompletionStatus(mHandle, OUT & bytes, OUT reinterpret_cast<PULONG_PTR>(&completionKey), OUT reinterpret_cast<LPOVERLAPPED*>(&overlapped), timeout))
	{
		client = reinterpret_cast<AsyncStream*>(completionKey);
		sockaddr_in* localAddr = NULL;
		sockaddr_in* remoteAddr = NULL;
		int localAddrLen = 0;
		int remoteAddrLen = 0;
		AsyncStream::GetAcceptExSockaddrs(
			client->mBuf,
			0, // 데이터를 수신하지 않았다면 0
			sizeof(SOCKADDR_IN) + 16,
			sizeof(SOCKADDR_IN) + 16,
			(sockaddr**) & localAddr,
			& localAddrLen,
			(sockaddr**) & remoteAddr,
			&remoteAddrLen
		);

		char ipString[INET6_ADDRSTRLEN];
		void* addr;

		if (remoteAddr->sin_family == AF_INET) {
			// IPv4
			struct sockaddr_in* ipv4 = (struct sockaddr_in*)remoteAddr;
			addr = &(ipv4->sin_addr);
		}
		else {
			// IPv6
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)remoteAddr;
			addr = &(ipv6->sin6_addr);
		}

		inet_ntop(remoteAddr->sin_family, addr, ipString, sizeof(ipString));
		std::cout << ipString << std::endl;
		//	char addrStr[INET_ADDRSTRLEN];
		//	inet_ntop(AF_INET, &remoteAddr->sin_addr, addrStr, sizeof(addrStr));

		//	std::cout << addrStr << " " << ntohs(remoteAddr->sin_port) << std::endl;
		
	}
	else
	{
		int error = WSAGetLastError();
		std::cout << error << std::endl;
	}

	return true;
}
