#include "pch.h"
#include "TcpStream.h"

TcpStream::TcpStream() : mSocket{0}, mRecvBytes{0}, mSendBytes{0}
{
	ZeroMemory(&mAddr, sizeof(mAddr));
	mRecvBuf.buf = static_cast<CHAR*>(XALLOCATE(2048));
	mRecvBuf.len = MAX_BUFF_SIZE;
	mSendBuf.buf = static_cast<CHAR*>(XALLOCATE(2048));
	mSendBuf.len = MAX_BUFF_SIZE;
}

TcpStream::~TcpStream()
{
	XRELEASE(mRecvBuf.buf);
	mRecvBuf.buf = nullptr;
	mRecvBuf.len = 0;
	XRELEASE(mSendBuf.buf);
	mSendBuf.buf = nullptr;
	mSendBuf.len = 0;
}

bool TcpStream::BindAny(uint16 port)
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr)) == 0;
}

bool TcpStream::Bind(std::string addr, uint16 port)
{
	TcpStream::SetAddr(addr, port);
	return bind(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr)) == 0;
}

auto TcpStream::Connect() -> bool
{
	return connect(mSocket, reinterpret_cast<PSOCKADDR>(&mAddr), sizeof(mAddr)) == 0;
}

bool TcpStream::Recv()
{
	return recv(mSocket, mRecvBuf.buf, mRecvBuf.len, 0) != SOCKET_ERROR;
}

bool TcpStream::Send(CHAR* msg, size_t size)
{
	return send(mSocket, mSendBuf.buf, mSendBuf.len, 0) != SOCKET_ERROR;
}

const SOCKET TcpStream::ConstGetSocket() const
{
	return mSocket;
}

SOCKET& TcpStream::GetSocketRef()
{
	return mSocket;
}

SOCKADDR_IN& TcpStream::GetAddrRef()
{
	return mAddr;
}

WSABUF& TcpStream::GetRecvBufRef()
{
	return mRecvBuf;
}

WSABUF& TcpStream::GetSendBufRef()
{
	return mSendBuf;
}

const DWORD TcpStream::GetRecvBytes() const
{
	return mRecvBytes;
}

const DWORD TcpStream::GetSendBytes() const
{
	return mSendBytes;
}

auto TcpStream::SetRecvBytes(DWORD bytes) -> void
{
	mRecvBytes = bytes;
}

auto TcpStream::SetSendBytes(DWORD bytes) -> void
{
	mSendBytes = bytes;
}

auto TcpStream::SetAddr(std::string addr, uint16 port) -> void
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	inet_pton(AF_INET, addr.c_str(), &mAddr);
}