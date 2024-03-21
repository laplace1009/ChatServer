#pragma once
#include "Types.h"

enum
{
	MAX_BUFF_SIZE = 2048,
};

class Network
{
public:
	virtual ~Network() = default;

public:
	virtual bool BindAny(uint16)										 = 0;
	virtual bool Bind(std::string, uint16)								 = 0;
	virtual bool Connect(DWORD* bytes)									 = 0;
	virtual bool Recv(WSABUF* buf, DWORD* bytes)						 = 0;
	virtual bool Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size) = 0;

public:
	virtual const SOCKET ConstGetSocket() const = 0;
	virtual SOCKET& GetSocketRef()				= 0;
	virtual SOCKADDR_IN& GetAddrRef()			= 0;
};

inline SOCKET CreateSocket() 
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

template<typename T>
static inline auto SetSocketOpt(Network* stream, int optName, T* optVal, size_t optLen) -> bool
{
	return ::setsockopt(stream->ConstGetSocket(), SOL_SOCKET, optName, reinterpret_cast<const char*>(optVal), static_cast<int>(optLen)) != SOCKET_ERROR;
}