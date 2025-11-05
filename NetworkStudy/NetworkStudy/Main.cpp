#include "Core.h"
using namespace std;

#include "GameCore.h"
#include "MemoryStream.h"
#include "Core.h"
#include <iostream>
#include "SocketUtil.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include <memory>


int Server2()
{
	// 주소
	SocketAddressPtr socketAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:12345");

	TCPSocketPtr serverSocket = SocketUtil::CreateTCPSocket(INET);
	serverSocket->Bind(*socketAddress);
	serverSocket->Listen();

	cout << "서버가 시작되었습니다. 클라이언트를 기다리는 중..." << endl;

	sockaddr tempsock{};
	SocketAddress clientAddr(tempsock);
	TCPSocketPtr clientSocket = serverSocket->Accept(clientAddr);

	cout << "클라이언트 접속" << '\n';

	char* tempBuffer = new char[1024];

	while (1)
	{
		int receivedCount = clientSocket->Receive(tempBuffer, 1024);

		if (receivedCount > 0)
		{
			InputMemoryStream stream(tempBuffer, uint32(receivedCount));
			int receivedData = 0;
			stream.Read(receivedData);
			cout << "수신 : " << receivedData << '\n';

			std::string message("서버로부터 응답");
			clientSocket->Send(message.c_str(), message.size());
		}
	}

	delete[] tempBuffer;
}

int Client2()
{
	// 주소
	SocketAddressPtr socketAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:12345");

	TCPSocketPtr clientSocket = SocketUtil::CreateTCPSocket(INET);

	// 서버에 연결
	clientSocket->Connect(*socketAddress);
	cout << "서버에 연결되었습니다." << endl;

	int count = 10;
	while (1)
	{
		if (GetAsyncKeyState('W') & 0x8000)
		{
			OutputMemoryStream ostream;
			ostream.Write(count);
			clientSocket->Send(ostream.GetBufferPtr(), ostream.GetLength());
			cout << "송신 : " << count << '\n';
			count++;

			// 응답 수신
			char* tempBuffer = new char[1024];
			int receivedCount = clientSocket->Receive(tempBuffer, 1024);
			if (receivedCount > 0)
			{
				tempBuffer[receivedCount] = '\0';
				cout << tempBuffer << '\n';
			}
			delete[] tempBuffer;
		}
	}
}

int main() {

	int server = 1;
	cin >> server;

	SocketUtil::StaticInit();

	if (server == 1)
		Server2();
	else
		Client2();

	SocketUtil::CleanUp();

	return 0;
}