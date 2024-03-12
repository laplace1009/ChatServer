#pragma once

enum
{
	MAX_BUFF_SIZE = 2048,
};

class Stream
{
public:
	virtual ~Stream() {}

public:
	virtual bool Bind() = 0;
	virtual bool Bind(std::string, uint16) = 0;
	virtual bool Connect() = 0;
	virtual bool Recv() = 0;
	virtual bool Send() = 0;
	
public:
	virtual const SOCKET ConstGetSocket() const			= 0;
	virtual void SetSocket(SOCKET socket)				= 0;
	virtual const SOCKADDR_IN& ConstGetAddrRef()		= 0;
	virtual bool SetAddr(std::string addr, uint16 port) = 0;
	virtual const WSABUF& ConstGetRecvBufRef()			= 0;
	virtual WSABUF& GetRecvBufRef()						= 0;
	virtual const DWORD ConstGetRecvBytes()				= 0;
	virtual void SetRecvBytes(DWORD bytes)				= 0;
	virtual const WSABUF& ConstGetSendBufRef()			= 0;
	virtual WSABUF& GetSendBufRef()						= 0;
	virtual const DWORD ConstGetSendBytes()				= 0;
	virtual void SetSendBytes(DWORD bytes)				= 0;
};

template<typename T>
static inline auto SetSocketOpt(Stream* stream, int optName, T* optVal, size_t optLen) -> bool
{
	::setsockopt(stream->ConstGetSocket(), SOL_SOCKET, optName, reinterpret_cast<const char*>(optVal), sizeof(T));
}


