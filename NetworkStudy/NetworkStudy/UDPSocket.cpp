#include "UDPSocket.h"
#include "SocketAddress.h"
#include "SocketUtil.h"

UDPSocket::~UDPSocket()
{
	closesocket(mSocket);
}

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
	int err = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());

	if (err == 0)
		return NO_ERROR;

	// 에러가 있을 경우
	SocketUtil::ReportError("UDPSocket::Bind");
	return SocketUtil::GetLastError();
}

int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo)
{
	// mSocket은 데이터그램을 보낼 소켓
	// 바인딩되지 않았다면 라이브러리가 자동으로 포트를 골라 바인딩함
	// 바인딩한 주소와 포트는 외부로 나가는 패킷 헤더의 발신자 주소가 된다.

	// inData는 보낼 데이터의 시작 주로를 가리키는 포인터. 
	// sendto함수에서는 char*을 인자로 받으니 캐스팅하여 전달
	// len은 데이터의 길이. 
	// 분열을 피하기 위해 데이터 그램의 길이는 1300바이트 이내로
	int byteSentCount = sendto(
		mSocket,
		static_cast<const char*>(inData),
		inLen,
		0, &inTo.mSockAddr, inTo.GetSize()
	);

	if (byteSentCount >= 0)
		return byteSentCount;

	// 에러 코드를 음수로 리턴함
	SocketUtil::ReportError("UDPSocket::SendTo");
	return -SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inMaxLength, SocketAddress& outFromAddress)
{
	int fromLength = outFromAddress.GetSize();
	int readByteCount = recvfrom(
		mSocket,
		static_cast<char*>(inBuffer),
		inMaxLength,
		0,
		&outFromAddress.mSockAddr,
		&fromLength
	);

	if (readByteCount >= 0)
		return readByteCount;

	SocketUtil::ReportError("UDPSocket::receiveFrom");
	return -SocketUtil::GetLastError();
}

int UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
#if _WIN32
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
#else
	int flags = fcntl(mSocket, F_GETFL, 0);
	flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	fcntl(mSocket, F_SETFL, flags);
#endif

	if (result != SOCKET_ERROR)
		return NO_ERROR;

	SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
	return SocketUtil::GetLastError();
}
