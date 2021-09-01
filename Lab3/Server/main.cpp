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
		return ID == client.ID;
	}
};

std::list<Client> clients;

sockaddr CreateAddress(std::string ip, uint16_t port);
void HandleError(std::string message);
void HandleClient(Client client);

int main()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		HandleError("Failed to initialize library.");

	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSock == INVALID_SOCKET)
		HandleError("Failed to create socket.");

	sockaddr addr = CreateAddress("127.0.0.1", 4790);

	if (bind(listenSock, &addr, sizeof(sockaddr)) != 0)
		HandleError("Failed to bind socket.");

	if (listen(listenSock, MaxConnections) != 0)
		HandleError("Failed to listen.");

	int count = 0;

	while (true)
		if (clients.size() < MaxConnections)
		{
			Client client;
			client.sock = accept(listenSock, reinterpret_cast<sockaddr*>(&client.addr), NULL);

			char name[ClientNameSize], clientIP[16];
			recv(client.sock, name, ClientNameSize, NULL);
			client.name = name;
			client.ID = count++;
			client.isConnected = true;
			clients.push_back(client);
			std::cout << client.name << " has been connected." << std::endl;
			inet_ntop(AF_INET, &client.addr, clientIP, 16);
			std::cout << "IP address: " << clientIP << std::endl;

			std::string tempMsg = client.name + " joined chat.";
			for (auto x : clients)
				if (client.ID != x.ID)
					send(x.sock, tempMsg.c_str(), MessageSize, NULL);

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
	std::string tempMsg;
	while (client.isConnected)
	{
		if (recv(client.sock, message, MessageSize, NULL) != SOCKET_ERROR)
		{
			std::cout << client.name << ": " << message << std::endl;

			if (strcmp(message, "exit") == 0)
				client.isConnected = false;

			tempMsg = client.name + ": " + message;

			for (auto x : clients)
				if (client.ID != x.ID)
					send(x.sock, tempMsg.c_str(), MessageSize, NULL);
		}
		else
			client.isConnected = false;
	}

	std::cout << client.name << " left the chat." << std::endl;
	clients.remove(client);

	tempMsg = client.name + " left the chat.";
	for (auto x : clients)
		send(x.sock, tempMsg.c_str(), MessageSize, NULL);

	closesocket(client.sock);
}