#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h>
#include <WS2tcpip.h>

const int MaxConnections = 50;
const int ClientNameSize = 20;
const int MessageSize = 512;

int count = 0;

struct Client
{
	char name[ClientNameSize];
	SOCKET sock;
	sockaddr addr;
	bool isConnected;
};

Client clients[MaxConnections];
std::thread clientThreads[MaxConnections];

sockaddr CreateAddress(std::string ip, uint16_t port);
void HandleError(std::string message);
void HandleClient(int clientID);

int main()
{
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		HandleError("Failed to initialize library.");

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
		HandleError("Failed to create socket.");

	sockaddr addr = CreateAddress("127.0.0.1", 4790);

	result = bind(listenSock, &addr, sizeof(sockaddr));
	if (result != 0)
		HandleError("Failed to bind socket.");

	result = listen(listenSock, MaxConnections);
	if (result != 0)
		HandleError("Failed to listen.");

	for (int i = 0; i < MaxConnections; i++)
	{
		clients[i].sock = accept(listenSock, &clients[i].addr, NULL);
		int result = recv(clients[i].sock, clients[i].name, ClientNameSize, NULL);
		if (result == SOCKET_ERROR)
			HandleError("Failed to receive client name.");

		std::cout << clients[i].name << " has been connected." << std::endl;
		std::cout << "IP address: " << std::endl;

		clients[i].isConnected = true;
		new std::thread(HandleClient, i);
		count++;
	}

	WSACleanup();
	system("pause");
	return 0;
}

sockaddr CreateAddress(std::string ip, uint16_t port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	return *reinterpret_cast<sockaddr*>(&addr);
}

void HandleError(std::string message)
{
	std::cout << "ERROR. " << message << std::endl;
	system("pause");
	exit(1);
}

void HandleClient(int clientID)
{
	char message[MessageSize];
	while (clients[clientID].isConnected)
	{
		int result = recv(clients[clientID].sock, message, MessageSize, NULL);
		if (result == SOCKET_ERROR)
			clients[clientID].isConnected = false;

		for (int i = 0; i < count; i++)
			if (i != clientID)
				int result = send(clients[i].sock, message, MessageSize, NULL);
	}
}