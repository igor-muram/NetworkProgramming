#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

const int BuffSize = 8192;

// Создание адреса
sockaddr CreateAddress(std::string ip, uint16_t port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	return *reinterpret_cast<sockaddr*>(&addr);
}

// Вывод ошибок
void HandleError(std::string msg)
{
	std::cout << "ERROR! " << msg << std::endl;
	system("pause");
	exit(EXIT_FAILURE);
}

int main()
{
	// Инициализация библиотки
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		HandleError("Failed to initialize library.");

	// Создание сокета
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Соединение с сервером
	std::string IP;
	std::cout << "IP > ";
	std::cin >> IP;
	sockaddr addr = CreateAddress(IP, 2005);

	if (connect(sock, &addr, sizeof(sockaddr)) != 0)
		HandleError("Failed to connect to server.");
	
	std::cout << "Connected to server successfully." << std::endl;

	// Отправка информации
	char data[BuffSize];
	std::cout << "Type directory name: ";
	std::cin >> data;

	if (send(sock, data, BuffSize, NULL) == SOCKET_ERROR)
		HandleError("Failed to send data.");
	
	std::cout << "Type string: ";
	std::cin >> data;

	if (send(sock, data, BuffSize, NULL) == SOCKET_ERROR)
		HandleError("Failed to send data.");

	// Получение результата с сервера
	if (recv(sock, data, BuffSize, NULL) == SOCKET_ERROR)
		HandleError("Failed to receive data from server.");

	std::cout << "Data from server:" << std::endl;
	std::cout << data << std::endl;

	closesocket(sock);

	WSACleanup();
	system("pause");
	return 0;
}