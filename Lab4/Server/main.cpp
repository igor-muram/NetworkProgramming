#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

const int BuffSize = 256;

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

	if (listen(listenSock, SOMAXCONN) != 0)
		HandleError("Failed to listen.");

	char data[BuffSize];
	while (true)
	{
		SOCKET clientSock = accept(listenSock, NULL, NULL);

		if (recv(clientSock, data, BuffSize, NULL) == SOCKET_ERROR)
			HandleError("Failed to receive data from client.");

		std::cout << "Data from client: " << data << std::endl;

		closesocket(clientSock);
	}

	WSACleanup();
	system("pause");
	return 0;
}