#pragma once
#include "IOCP.h"

class Client : public IOCP
{
public:
	Client();
	~Client();

public:
	UN_USED Error Register(LPAsyncEndpoint handle)	override;
	Error Dispatch()								override;

public:
	auto Send(uint16 protocol, uint16 size, CHAR* msg) -> Error;
	auto SetRecv() -> Error;

private:
	auto doIOAction() -> void;
	auto setEventConnect() -> void;
	auto setEventRecv() -> void;
	auto setEventSend() -> void;
	auto setEventDisconnect() -> void;
	auto afterIOConnectEvent() -> void;
	auto afterIORecvEvent() -> void;
	auto afterIOSendEvent() -> void;
	auto afterIODisconnect() -> void;

private:
	LPAsyncEndpoint	mClient;
	HANDLE			mHandle;
};

