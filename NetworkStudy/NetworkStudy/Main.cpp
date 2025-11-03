#include "Core.h"
using namespace std;

#include "GameCore.h"
#include "MemoryStream.h"
#include "GameObject.h"

#include "SocketUtil.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include <memory>

std::shared_ptr<GameObject> gGameObject;
std::shared_ptr<GameObject> gTempGameObject;

void SendGameObject(int inSocket, const std::shared_ptr<GameObject> inGameObject);
void ReceiveGameObject(int inSocket, std::shared_ptr<GameObject>& outGameObject);

void Initialize(std::string inName)
{
	gGameObject = std::make_shared<GameObject>(inName);
	gTempGameObject = std::make_shared<GameObject>("ReceivedGameObject");
}

void GameLoop(int inSocket)
{
	if (gGameObject->GetName() == "Client")
	{
		if (GetAsyncKeyState('E') & 0x8000)
		{
			SendGameObject(inSocket, gGameObject);
		}

		if (GetAsyncKeyState('W') & 0x8000)
		{
			gGameObject->AddCount();
		}
	}
	if (gGameObject->GetName() == "Server")
	{
		ReceiveGameObject(inSocket, gTempGameObject);
	}
}

void SendGameObject(int inSocket, const std::shared_ptr<GameObject> inGameObject)
{
	OutputMemoryStream stream;
	inGameObject->Write(stream);
	if (send(inSocket, stream.GetBufferPtr(), stream.GetLength(), 0) == SOCKET_ERROR)
	{
		cerr << "전송 실패: " << WSAGetLastError() << endl;
		return;
	}

	// 응답 수신
	char* temporaryBuffer = new char[](1024);
	uint64 receivedByteCount = recv(inSocket, temporaryBuffer, 1024, 0);

	if (receivedByteCount <= 0)
	{
		if (receivedByteCount == 0)
		{
			cout << "서버 연결 종료" << endl;
		}
		else
		{
			cerr << "수신 실패: " << WSAGetLastError() << endl;
		}

		delete[]temporaryBuffer;
		return;
	}

	temporaryBuffer[receivedByteCount] = '\0';
	cout << format("서버로부터 응답: {}\n", temporaryBuffer);
	delete[]temporaryBuffer;
}

const uint32 g_MaxPacketSize = 1470;

void ReceiveGameObject(int inSocket, std::shared_ptr<GameObject>& outGameObject)
{
	char* temporaryBuffer = new char[](g_MaxPacketSize);
	uint64 receivedByteCount = recv(inSocket, temporaryBuffer, g_MaxPacketSize, 0);

	if (receivedByteCount > 0)
	{
		InputMemoryStream stream(temporaryBuffer, static_cast<uint32>(receivedByteCount));

		outGameObject->Read(stream);
		outGameObject->SayInfo();

		string response = "메시지 수신 완료";

		if (send(inSocket, response.c_str(), response.length(), 0) == SOCKET_ERROR) {
			cerr << "전송 실패: " << WSAGetLastError() << endl;
		}
	}
	delete[] temporaryBuffer;
}


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
		OutputMemoryStream stream;
		stream.Write(count);
		clientSocket->Send(stream.GetBufferPtr(), stream.GetLength());
		cout << "송신 : " << count << '\n';
		count++;

		// 응답 수신
		char* tempBuffer = new char[1024 + 1];
		int receivedCount = clientSocket->Receive(tempBuffer, 1024);
		if (receivedCount > 0)
		{
			tempBuffer[receivedCount] = '\0';

			InputMemoryStream stream(tempBuffer, uint32(receivedCount));
			stream.Read(tempBuffer, receivedCount);
			cout << tempBuffer << '\n';
		}
		delete[] tempBuffer;
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