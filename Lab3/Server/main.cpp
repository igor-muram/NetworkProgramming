#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <cstring>
#include <WinSock2.h>
#include <WS2tcpip.h>

const int MaxConnections = 20;
const int ClientNameSize = 20;
const int MessageSize = 512;

struct Client
{
	std::string name;
	int ID;
	sockaddr_in addr;
	SOCKET sock;
	bool isConnected;

	bool operator==(Client client)
	{
		return ID = client.ID;
	}
};

std::list<Client> clients;

sockaddr CreateAddress(std::string ip, uint16_t port);
void HandleError(std::string message);
void HandleClient(Client client);

int main()
{
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		HandleError("Failed to initialize library.");

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
		HandleError("Failed to create socket.");

	sockaddr addr = CreateAddress("192.168.0.11", 4790);

	result = bind(listenSock, &addr, sizeof(sockaddr));
	if (result != 0)
		HandleError("Failed to bind socket.");

	result = listen(listenSock, MaxConnections);
	if (result != 0)
		HandleError("Failed to listen.");

	int count = 0;

	while (clients.size() < MaxConnections)
	{
		Client client;
		client.sock = accept(listenSock, reinterpret_cast<sockaddr*>(&client.addr), NULL);

		char name[ClientNameSize];
		recv(client.sock, name, ClientNameSize, NULL);
		client.name = name;
		client.ID = count++;
		client.isConnected = true;
		clients.push_back(client);
		std::cout << client.name << " has been connected." << std::endl;
		new std::thread(HandleClient, client);
	}

	system("pause");
	WSACleanup();
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
	WSACleanup();
	system("pause");
	exit(1);
}

void HandleClient(Client client)
{
	char message[MessageSize];
	int result = 0;
	while (client.isConnected)
	{
		result = recv(client.sock, message, MessageSize, NULL);

		if (result == SOCKET_ERROR)
			client.isConnected = false;

		if (strcmp(message, "exit") == 0)
			client.isConnected = false;

		if (client.isConnected)
			for (auto x : clients)
				if (client.ID != x.ID)
					send(x.sock, message, MessageSize, NULL);

	}

	std::cout << client.name << " left the chat." << std::endl;
	clients.remove(client);
	closesocket(client.sock);
}