#include "SocketAddressFactory.h"

SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const std::string& inString)
{
	// --------이름에서 클론을 찾아 포트 번호 분리--------- //
	auto pos = inString.find_last_of(':');
	std::string host, service;
	if (pos != std::string::npos)
	{
		host = inString.substr(0, pos);	
		service = inString.substr(pos + 1); 
	}
	else
	{
		// 포트가 지정되지 않았으므로 디폴트를 사용함
		host = inString;
		service = "0";
	}
	addrinfo hint{};
	hint.ai_family = AF_INET;

	addrinfo* result = nullptr;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
	addrinfo* initResult = result;

	if (error != 0 && result != nullptr)
	{
		freeaddrinfo(initResult);
	}

	while (!result->ai_addr && result->ai_next)
	{
		result = result->ai_next;
	}

	if (!result->ai_addr)
	{
		freeaddrinfo(initResult);
		return nullptr;
	}

	auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

	freeaddrinfo(initResult);
	return toRet;
}
