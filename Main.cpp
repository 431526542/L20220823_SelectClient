#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<iostream>
#include<WinSock2.h>
#include<process.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int Gold = 0;

CRITICAL_SECTION GoldSection;
SOCKET ServerSocket;

unsigned __stdcall RecvThread(void* Arg)
{
	while (true)
	{
		char Buffer2[1024] = { 0, };
		recv(ServerSocket, Buffer2, 1024, 0);

		cout << Buffer2 << endl;
	}
	return 0;
}

unsigned __stdcall SendThread(void* Arg)
{
	char Buffer[1024] = { 0, };
	bool isRunning = true;
	while (isRunning)
	{
		cin >> Buffer;

		if (strcmp(Buffer, "quit") == 0)
		{
			return 0;
		}

		EnterCriticalSection(&GoldSection);
		send(ServerSocket, Buffer, strlen(Buffer), 0);
		LeaveCriticalSection(&GoldSection);
	}

	return 0;
}

int main()
{
	/*
	WSAData WsaData;

	WSAStartup(MAKEWORD(2, 2), &WsaData);

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.10.141");
	ServerSockAddr.sin_port = htons(12345);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(SOCKADDR_IN));

	while (1)
	{
		char Buffer[1024] = { 0, };
		cin >> Buffer;

		send(ServerSocket, Buffer, strlen(Buffer), 0);

		char Buffer2[1024] = { 0, };
		recv(ServerSocket, Buffer2, 1024, 0);

		cout << Buffer2 << endl;
	}

	closesocket(ServerSocket);


	WSACleanup();*/

	WSAData WsaData;

	WSAStartup(MAKEWORD(2, 2), &WsaData);

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.s_addr = inet_addr("192.168.10.141");
	ServerSockAddr.sin_port = htons(12345);

	connect(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(SOCKADDR_IN));

	InitializeCriticalSection(&GoldSection);

	HANDLE RecvThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, RecvThread, nullptr, 0, nullptr);

	HANDLE SendThreadHandle = (HANDLE)_beginthreadex(nullptr, 0, SendThread, nullptr, 0, nullptr);

	//WaitForSingleObject(RecvThreadHandle, INFINITE); //RecvThreadHandle는 (무한정) 기달려라
	WaitForSingleObject(SendThreadHandle, INFINITE);

	DeleteCriticalSection(&GoldSection);

	closesocket(ServerSocket);

	WSACleanup();

	return 0;

}