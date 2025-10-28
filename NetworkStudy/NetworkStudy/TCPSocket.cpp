#include "TCPSocket.h"
#include "SocketUtil.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Connect(const SocketAddress& inAddress)
{
	int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err >= 0)
		return NO_ERROR;

	SocketUtil::ReportError("TCPSocket::Connect");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Bind(const SocketAddress& inToAddress)
{
	// 포트 번호를 바인드
	int err = bind(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());

	if (err == 0)
		return NO_ERROR;

	// 에러가 있을 경우
	SocketUtil::ReportError("TCPSocket::Bind");
	return SocketUtil::GetLastError();
}

int TCPSocket::Listen(int inBackLog /*= 32*/)
{
	// backlog : 대기열 
	int err = listen(mSocket, inBackLog);
	if (err >= 0)
		return NO_ERROR;

	SocketUtil::ReportError("TCPSocket::Listen");
	return -SocketUtil::GetLastError();
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress)
{
	int length = inFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

	if (newSocket != INVALID_SOCKET)
		return TCPSocketPtr(new TCPSocket(newSocket));

	SocketUtil::ReportError("TCPSocket::Accept");
	return nullptr;
}

int TCPSocket::Send(const void* inData, int inLen)
{
	int bytesSentCount = send(
		mSocket,
		static_cast<const char*>(inData),
		inLen,
		0
	);

	if (bytesSentCount >= 0)
		return bytesSentCount;

	SocketUtil::ReportError("TCPSocket::Send");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Receive(void* inBuffer, int inLen)
{
	int bytesReceivedCount = recv(
		mSocket,
		static_cast<char*>(inBuffer),
		inLen,
		0
	);

	if (bytesReceivedCount >= 0)
		return bytesReceivedCount;

	SocketUtil::ReportError("TCPSocket::Receive");
	return -SocketUtil::GetLastError();
}
