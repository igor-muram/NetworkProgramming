#include <iostream>
#include <string>
#include <thread>
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
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
		HandleError("Failed to initialize library.");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr addr = CreateAddress("127.0.0.1", 4790);

	result = connect(sock, &addr, sizeof(sockaddr));
	if (result != 0)
		HandleError("Failed to connect to server.");

	char name[ClientNameSize];
	std::cout << "Enter your name: ";
	std::cin >> name;

	result = send(sock, name, ClientNameSize, NULL);
	if (result == SOCKET_ERROR)
		HandleError("Failed to send data to server.");

	char message[MessageSize];
	std::thread thread(HandleIncomingMsg, sock);
	while (result != SOCKET_ERROR)
	{
		std::cin >> message;
		result = send(sock, message, MessageSize, NULL);
	}

	closesocket(sock);

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

void HandleIncomingMsg(SOCKET sock)
{
	char message[MessageSize];
	recv(sock, message, MessageSize, NULL);
	std::cout << message << std::endl;
}