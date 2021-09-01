#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

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

// Поиск файлов с заданной подстрокой
void findFiles(std::string dirName, std::string str, std::string& result)
{
	std::string dirFiles = dirName + "/*";
	std::stringstream resultStream;

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(dirFiles.c_str(), &data);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			std::string filename = data.cFileName;
			if (filename.ends_with("txt"))
			{
				std::ifstream in(dirName + "/" + filename, std::ios::in);

				std::stringstream buffer;
				buffer << in.rdbuf();

				if (buffer.str().find(str) != std::string::npos)
					resultStream << filename << " ";
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);

		result = resultStream.str();
		if (result.empty())
			result = "No files with such string.";
	}
	else
	{
		result = "ERROR! Failed to find directory.";
	}
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
	// Инициализация библиотеки
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		HandleError("Failed to initialize library.");

	// Создание сокета
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, NULL);
	if (listenSock == INVALID_SOCKET)
		HandleError("Failed to create socket.");

	// Привязка сокета к IP адресу и порту
	std::string IP;
	std::cout << "IP > ";
	std::cin >> IP;

	sockaddr addr = CreateAddress(IP, 2005);
	if (bind(listenSock, &addr, sizeof(sockaddr)) != 0)
		HandleError("Failed to bind socket.");

	if (listen(listenSock, SOMAXCONN) != 0)
		HandleError("Failed to listen.");
	
	std::cout << "Listen to the port 2005. Waiting for connections." << std::endl;
	
	char string[BuffSize];
	char dirName[BuffSize];
	std::string result;

	while (true)
	{
		// Обработка подключения
		SOCKET clientSock = accept(listenSock, NULL, NULL);

		std::cout << "Client has connected." << std::endl;

		// Прием информации, отправленной клиентом
		if (recv(clientSock, dirName, BuffSize, NULL) == SOCKET_ERROR)
			HandleError("Failed to receive data from client.");

		if (recv(clientSock, string, BuffSize, NULL) == SOCKET_ERROR)
			HandleError("Failed to receive data from client.");

		std::cout << "Data from client:" << std::endl;
		std::cout << "Directory name: " << dirName << std::endl;
		std::cout << "String: " << string << std::endl;

		// Поиск файлов
		findFiles(dirName, string, result);
		std::cout << "Result: " <<  result << std::endl;

		// Отправление результата обратно к клиенту
		if (send(clientSock, result.c_str(), BuffSize, NULL) == SOCKET_ERROR)
			HandleError("Failed to send data to client.");
		
		std::cout << "Sending data back to client.\n\n\n";

		closesocket(clientSock);
	}

	WSACleanup();
	return 0;
}