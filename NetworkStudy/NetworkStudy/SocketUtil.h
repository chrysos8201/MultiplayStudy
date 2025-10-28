#pragma once
#include "UDPSocket.h"

enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil
{
public:
	static void ReportError(const char* inOperationDesc);
	static int GetLastError();

	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
};

