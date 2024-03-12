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

auto TcpStream::CreateSocket() -> int
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
}


auto TcpStream::Bind() -> bool
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(0);
	mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
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

bool TcpStream::Send()
{
	return send(mSocket, mSendBuf.buf, mSendBuf.len, 0) != SOCKET_ERROR;
}

const SOCKET TcpStream::ConstGetSocket() const
{
	return mSocket;
}

void TcpStream::SetSocket(SOCKET socket)
{
	mSocket = socket;
}

const SOCKADDR_IN& TcpStream::ConstGetAddrRef()
{
	return mAddr;
}

bool TcpStream::SetAddr(std::string addr, uint16 port)
{
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(port);
	return inet_pton(AF_INET, addr.c_str(), &mAddr.sin_addr) == 1;
}

const WSABUF& TcpStream::ConstGetRecvBufRef()
{
	return mRecvBuf;
}

WSABUF& TcpStream::GetRecvBufRef()
{
	return mRecvBuf;
}

const DWORD TcpStream::ConstGetRecvBytes()
{
	return mRecvBytes;
}

void TcpStream::SetRecvBytes(DWORD size)
{
	mRecvBytes = size;
}

const DWORD TcpStream::ConstGetSendBytes()
{
	return mSendBytes;
}

void TcpStream::SetSendBytes(DWORD bytes)
{
	mSendBytes = bytes;
}