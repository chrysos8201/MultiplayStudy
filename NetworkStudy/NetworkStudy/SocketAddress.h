#pragma once
#include "Core.h"

class SocketAddress
{
public:

	// 4바이트 IP주소와 포트 번호를 받아 sockaddr 값을 채움
	// IPv4 전용
	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET; // AF_INET은 IPv4 주소 체계

		// 여러 바이트로 된 숫자를 호스트의 순서가 아닌 네트워크 순서 체계로 변환
		// 호스트의 바이트 순서와 네트워크 바이트 순서가 같은 플랫폼에서 작동X

		// IP 주소 지정
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress); // 부호 없는 32비트 정수를 받아 호스트의 네이티브 바이트 순서->네트워크 바이트 순서로 변환

		// 포트 지정
		GetAsSockAddrIn()->sin_port = htons(inPort); // 부호 없는 16비트 정수를 받아 호스트의 네이티브 바이트 순서->네트워크 바이트 순서로 변환
	}

	// 네트워크API가 sockaddr을 리턴할 경우 래핑해서 사용할 때를 위한 생성자
	SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	size_t GetSize() const { return sizeof(sockaddr); }

private:
	friend class UDPSocket;
	friend class TCPSocket;

	// 소켓 주소. 
	sockaddr mSockAddr;

	sockaddr_in* GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;
