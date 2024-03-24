#include "pch.h"
#include "Client.h"

Client::Client()
{
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(mHandle != NULL);
	ASSERT_CRASH(AsyncStream::Init() != Error::OK);
	Register(mClient);
}

Client::~Client()
{
	CloseHandle(mHandle);
}

Error Client::Register(LPAsyncEndpoint handle)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(handle->ConstGetSocket()), mHandle, 0, 0))
		return Error::IOCP_REGISTER_ERROR;

	return Error::OK;
}

Error Client::Dispatch()
{
	ULONG_PTR key;
	DWORD transferred = 0;
	LPOVERLAPPEDEX retOver = nullptr;
	LPAsyncEndpoint client = nullptr;
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(&retOver), 1000))
	{
		client = reinterpret_cast<LPAsyncEndpoint>(retOver->owner);

		doIOAction();
	}
	else
	{
		int32 error = WSAGetLastError();
		switch (error)
		{
		case WAIT_TIMEOUT:
			return Error::IOCP_DISPATCH_ERROR;
		default:

		}
	}
	return Error::OK;
}

auto Client::Send(uint16 protocol, uint16 size, CHAR* msg) -> Error
{
	uint16 packetHeaderSize = sizeof(PacketHeader);
	PacketHeader header{ protocol, size + packetHeaderSize };
	WSABUF buf;
	buf.buf = new CHAR[size + packetHeaderSize];
	buf.len = size + packetHeaderSize;
	memcpy_s(buf.buf, buf.len, &header, packetHeaderSize);
	memcpy_s(buf.buf + packetHeaderSize, buf.len - packetHeaderSize, msg, size);
	
	return mClient->Send(&buf);
}

auto Client::SetRecv() -> Error
{
	if (mClient->Recv() == Error::NET_RECV_ERROR)
		return Error::NET_RECV_ERROR;
	setEventRecv();

	return Error::OK;
}

auto Client::doIOAction() -> void
{
	IOEvent ioEvent = mClient->GetEndpointRef().GetIOEvent();
	switch (ioEvent)
	{
	case IOEvent::CONNECT:
		afterIOConnectEvent();
		return;
	case IOEvent::RECV:
		afterIORecvEvent();
		return;
	case IOEvent::SEND:
		afterIOSendEvent();
		return;
	case IOEvent::DISCONNECT:
		afterIODisconnect();
		return;
	default:
		CRASH("Do not reach this case");
		return;
	}
}

auto Client::setEventConnect() -> void
{
	mClient->GetEndpointRef().SetIOEvent(IOEvent::CONNECT);
}

auto Client::setEventRecv() -> void
{
	mClient->GetEndpointRef().SetIOEvent(IOEvent::RECV);
}

auto Client::setEventSend() -> void
{
	mClient->GetEndpointRef().SetIOEvent(IOEvent::SEND);
}

auto Client::setEventDisconnect() -> void
{
	mClient->GetEndpointRef().SetIOEvent(IOEvent::DISCONNECT);
}

auto Client::afterIOConnectEvent() -> void
{
	mClient->SocketConnectUpdate();
	if (SetRecv() == Error::NET_RECV_ERROR)
		return;
}

auto Client::afterIORecvEvent() -> void
{
	PacketHeader header = *reinterpret_cast<PacketHeader*>(mClient->GetBufRef().buf);
	// 상황에 맞게 구현
}

auto Client::afterIOSendEvent() -> void
{
	setEventRecv();
	ZeroMemory(mClient->GetBufRef().buf, MAX_BUFF_SIZE);
	mClient->GetBufRef().len = MAX_BUFF_SIZE;
}

auto Client::afterIODisconnect() -> void
{
	setEventDisconnect();
}
