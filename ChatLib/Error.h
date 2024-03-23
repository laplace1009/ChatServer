#pragma once

enum class Error
{
	OK,
	NET_WSA_INIT_ERROR = 1,
	NET_CREATE_SOCKET_ERROR,
	NET_SOCKET_OPT_ERROR,
	NET_BIND_ERROR,
	NET_LISTEN_ERROR,
	NET_ACCEPT_ERROR,
	NET_CONNECT_ERROR,
	NET_RECV_ERROR,
	NET_SEND_ERROR,
	NET_BIND_LPFN_ERROR,
	IOCP_REGISTER_ERROR,
	IOCP_DISPATCH_ERROR,
	SERVER_EMPTY_SENDBUFFS,
};