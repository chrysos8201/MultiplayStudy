#pragma once
#include "Core.h"

class SocketAddress;
class TCPSocket;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;

class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inBindAddress);
	int Listen(int inBackLog = 32);
	TCPSocketPtr Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inData, int inLen);

private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};

