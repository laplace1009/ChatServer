#include "pch.h"
#include "Client.h"

Client::Client()
{
	mClient = xnew<AsyncEndpoint>();
	mHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(mHandle != NULL);
	ASSERT_CRASH(AsyncStream::Init() == Error::OK);
	Register(mClient);
}

Client::~Client()
{
	xdelete<AsyncEndpoint>(mClient);
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
	if (GetQueuedCompletionStatus(mHandle, &transferred, &key, reinterpret_cast<LPOVERLAPPED*>(&retOver), 1000))
	{
		doIOAction();
	}
	else
	{
		int32 error = WSAGetLastError();
		switch (error)
		{
		case WAIT_TIMEOUT:
			return Error::OK;
		default:
			return Error::IOCP_DISPATCH_ERROR;
		}
	}
	return Error::OK;
}

auto Client::Connect(String addr, uint16 port) -> Error
{
	if (mClient->BindAny(0) == Error::NET_BIND_ERROR)
		return Error::NET_BIND_ERROR;

	mClient->SetAddr(addr, port);

	return mClient->Connect();
}

auto Client::Send(uint16 protocol, uint16 size, CHAR* msg) -> Error
{
	uint16 packetHeaderSize = static_cast<uint16>(sizeof(PacketHeader));
	PacketHeader header{ size + packetHeaderSize, protocol };
	WSABUF buf;
	buf.buf = new CHAR[size + packetHeaderSize];
	buf.len = size + packetHeaderSize;
	memcpy_s(buf.buf, buf.len, &header, packetHeaderSize);
	memcpy_s(buf.buf + packetHeaderSize, buf.len - packetHeaderSize, msg, size);
	setEventSend();

	return mClient->Send(&buf);
}

auto Client::SetRecv() -> Error
{
	ZeroMemory(mClient->GetBufRef().buf, MAX_BUFF_SIZE);
	mClient->GetBufRef().len = MAX_BUFF_SIZE;
	setEventRecv();
	if (mClient->Recv() == Error::NET_RECV_ERROR)
		return Error::NET_RECV_ERROR;

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
	std::cout << "Connect Server!" << std::endl;
	if (mClient->SocketConnectUpdate() != Error::NET_CONNECT_ERROR)
	
	if (SetRecv() == Error::NET_RECV_ERROR)
		return;
}

auto Client::afterIORecvEvent() -> void
{
	PacketHeader header = *reinterpret_cast<PacketHeader*>(mClient->GetBufRef().buf);
	char* buf = new char[header.size - sizeof(PacketHeader) + 1];
	memcpy_s(buf, header.size - sizeof(PacketHeader), mClient->GetBufRef().buf + sizeof(PacketHeader), header.size - sizeof(PacketHeader));
	buf[header.size - sizeof(PacketHeader)] = '\0';
	std::cout << "Chat Server: " << buf << std::endl;
	if (Error::NET_RECV_ERROR == SetRecv())
		std::cerr << "Client Recv Error" << std::endl;
}

auto Client::afterIOSendEvent() -> void
{
	SetRecv();
}

auto Client::afterIODisconnect() -> void
{
	setEventDisconnect();
}
