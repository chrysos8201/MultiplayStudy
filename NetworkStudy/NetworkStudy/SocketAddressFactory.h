#pragma once
#include "Core.h"
#include "SocketAddress.h"

class SocketAddressFactory
{
public:

	// 이름이 잘못된 경우 nullptr 반환
	static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};

