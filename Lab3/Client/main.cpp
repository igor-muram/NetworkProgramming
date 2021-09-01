#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <WinSock2.h>
#include <WS2tcpip.h>

const int ClientNameSize = 20;
const int MessageSize = 512;

void HandleIncomingMsg(SOCKET sock);
sockaddr CreateAddress(std::string ip, uint16_t port);
void HandleError(std::string message);

int main()
{
	WSAData wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		HandleError("Failed to initialize library.");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr addr = CreateAddress("127.0.0.1", 4790);

	if (connect(sock, &addr, sizeof(sockaddr)) != 0)
		HandleError("Failed to connect to server.");

	char name[ClientNameSize];
	std::cout << "Enter your name: ";
	std::cin >> name;

	if (send(sock, name, ClientNameSize, NULL) == SOCKET_ERROR)
		HandleError("Failed to send data to server.");

	char message[MessageSize];
	std::thread thread(HandleIncomingMsg, sock);
	bool isConnected = true;
	while (isConnected)
	{
		std::cin >> message;
		if (send(sock, message, MessageSize, NULL) == SOCKET_ERROR)
			isConnected = false;

		if (strcmp(message, "exit") == 0)
			isConnected = false;
	}

	closesocket(sock);

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
	system("pause");
	exit(1);
}

void HandleIncomingMsg(SOCKET sock)
{
	char message[MessageSize];
	int result = 0;
	while (result != SOCKET_ERROR)
	{
		result = recv(sock, message, MessageSize, NULL);
		if (result != SOCKET_ERROR)
			std::cout << message << std::endl;
	}
}