#pragma once
#include "Types.h"
#include "Error.h"

enum
{
	MAX_BUFF_SIZE = 2048,
};

class Network
{
public:
	virtual ~Network() = default;

public:
	virtual Error BindAny(uint16)											= 0;
	virtual Error Bind(std::string, uint16)									= 0;
	virtual Error Connect(DWORD* bytes)										= 0;
	virtual Error Recv(WSABUF* buf, DWORD* bytes)							= 0;
	virtual Error Send(WSABUF* buf, DWORD* bytes, CHAR* msg, size_t size)	= 0;

public:
	virtual const SOCKET ConstGetSocket() const = 0;
	virtual SOCKET& GetSocketRef()				= 0;
	virtual SOCKADDR_IN& GetAddrRef()			= 0;
};

inline Error NetworkInit()
{
	WSADATA wsa;

	return WSAStartup(MAKEWORD(2, 2), &wsa) != 0 ? Error::NET_WSA_INIT_ERROR : Error::OK;
}

inline SOCKET CreateSocket() 
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

template<typename T>
static inline auto SetSocketOpt(Network* stream, int optName, T* optVal, size_t optLen) -> Error
{
	return ::setsockopt(stream->ConstGetSocket(), SOL_SOCKET, optName, reinterpret_cast<const char*>(optVal), static_cast<int>(optLen)) != SOCKET_ERROR ? Error::OK : Error::NET_SOCKET_OPT_ERROR;
}