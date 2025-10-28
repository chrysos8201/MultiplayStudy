#pragma once
#include "Core.h"
#include "SocketAddress.h"

class TCPSocket;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;

// TCP
// 신뢰성을 보장하며
// 데이터를 주고받기 위해 두 호스트 사이에  연결을 맺어 두어야 한다.
// 누락된 패킷을 재전송하기 위해 상태 정보를 유지하고 이를 어딘가에 저장해 두어야 한다.
class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog = 32);
	TCPSocketPtr Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);

private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}

private:
	SOCKET mSocket;
};

