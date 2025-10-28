#pragma once
#include "Core.h"

class SocketAddress;

// UDP
// 내부 상태가 없고 연결을 유지하지 않으며 신뢰성을 보장하지 않는다
// ->호스트마다 하나의 소켓만 있으면 데이터그램을 보내고 받기를 다 할 수 있다.
class UDPSocket
{
public:
	~UDPSocket();
	int Bind(const SocketAddress& inToAddress);

	// 데이터를 보낸다.
	int SendTo(const void* inData, int inLen, const SocketAddress& inTo);

	// UDP소켓으로 데이터를 받는 함수
	// inBuffer는 수신한 데이터그램을 복사해 둘 버퍼이다.
	// inLen. recvfrom()은 여기에 지정된 숫자 이상의 바이트는 복사하지 않고 
	// 모자라 잘린 부분은 버려지므로 넉넉히 잡는다.
	// inBuffer에 복사한 바이트의 길이를 반환한다.
	int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);

private:
	friend class SocketUtil;
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}

private:
	SOCKET mSocket;
};

using UDPSocketPtr = std::shared_ptr<UDPSocket>;

