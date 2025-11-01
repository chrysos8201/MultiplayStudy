#include <iostream>
#include <string>
#include <format>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#include "GameCore.h"
#include "MemoryStream.h"
#include "GameObject.h"

std::shared_ptr<GameCore> g_GameCore;

void Initialize(std::string inName)
{
	g_GameCore = std::make_shared<GameCore>();
	g_GameCore->Initialize(inName);
}

void GameLoop()
{
	g_GameCore->GameLoop();
}

void SendGameObject(int inSocket, const std::shared_ptr<GameObject> inGameObject)
{
	OutputMemoryStream stream;
	inGameObject->Write(stream);
	send(inSocket, stream.GetBufferPtr(), stream.GetLength(), 0);
}

int Server()
{
	// Winsock 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAStartup 실패" << endl;
		return 1;
	}

	// 소켓 생성
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cerr << "소켓 생성 실패: " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	// 소켓 주소 설정
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP 주소 수신
	serverAddr.sin_port = htons(12345); // 포트 12345 사용

	// 소켓 바인딩
	if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "바인딩 실패: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	// 리스닝 상태로 전환
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "리스닝 실패: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	cout << "서버가 시작되었습니다. 클라이언트를 기다리는 중..." << endl;


	// 클라이언트 연결 수락
	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);

	if (clientSocket == INVALID_SOCKET) {
		cerr << "클라이언트 연결 수락 실패: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}

	// 클라이언트 IP 출력
	char clientIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
	cout << format("클라이언트 연결됨: {}:{}\n", clientIP, ntohs(clientAddr.sin_port));

	// 데이터 수신 및 응답
	const int bufferSize = 1024;
	char buffer[bufferSize];

	Initialize("Server");

	while (true) 
	{
		GameLoop();

		// 데이터 수신
		int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
		if (bytesReceived <= 0) 
		{
			if (bytesReceived == 0) 
			{
				cout << "클라이언트 연결 종료" << endl;
			}
			else {
				cerr << "수신 실패: " << WSAGetLastError() << endl;
			}
			break;
		}

		// 수신된 데이터 처리
		buffer[bytesReceived] = '\0';
		cout << format("클라이언트로부터 수신: {}\n", buffer);

		// 응답 전송
		string response = "메시지 수신 완료: ";
		response += buffer;

		if (send(clientSocket, response.c_str(), response.length(), 0) == SOCKET_ERROR) {
			cerr << "전송 실패: " << WSAGetLastError() << endl;
			break;
		}

	}

	// 소켓 닫기
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();

}

int Client()
{
	// Winsock 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cerr << "WSAStartup 실패" << endl;
		return 1;
	}

	// 소켓 생성
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "소켓 생성 실패: " << WSAGetLastError() << endl;
		WSACleanup();
		return 1;
	}

	// 서버 주소 설정
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(12345); // 서버와 동일한 포트 사용

	// 서버 IP 주소 설정 (이 예제에서는 로컬호스트)
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
		cerr << "잘못된 서버 주소" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	// 서버에 연결
	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		cerr << "서버 연결 실패: " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	cout << "서버에 연결되었습니다." << endl;

	// 메시지 송수신
	const int bufferSize = 1024;
	char buffer[bufferSize];
	string message;

	Initialize("Client");

	while (true) 
	{
		//// 사용자로부터 메시지 입력 받기
		//cout << "서버로 보낼 메시지 (종료하려면 'exit' 입력): ";
		////getline(cin, message);
		//cin >> message;
		//
		//if (message == "exit") 
		//{
		//	break;
		//}
		GameLoop();

		//if (GetAsyncKeyState('Q') & 0x8000)
		//{
		//	break;
		//}
		//
		//if (GetAsyncKeyState('W') & 0x8000)
		//{
		//	message = 'W';
		//}

		if (message != "")
		{
			// 메시지 전송
			if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
				cerr << "전송 실패: " << WSAGetLastError() << endl;
				break;
			}

			// 응답 수신
			int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);
			if (bytesReceived <= 0) {
				if (bytesReceived == 0) {
					cout << "서버 연결 종료" << endl;
				}
				else {
					cerr << "수신 실패: " << WSAGetLastError() << endl;
				}
				break;
			}

			// 수신된 응답 처리
			buffer[bytesReceived] = '\0';
			cout << format("서버로부터 응답: {}\n", buffer);

			message = "";
		}

	}

	// 소켓 닫기
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}


int main() {

	int server = 1;
	cin >> server;
	if (server == 1)
		Server();
	else
		Client();
	return 0;
}