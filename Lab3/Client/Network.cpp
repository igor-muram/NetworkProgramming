#include "Network.h"

bool Connected = false;
mutex Mutex;

bool ConnectToServer(SOCKET& sock, sockaddr& addr)
{
	// Попытка подключиться к серверу
	if (connect(sock, &addr, sizeof(sockaddr)) != 0)
		return false;

	// Отправка имени клиента серверу
	char message[MsgSize];
	cout << "Enter your name: ";
	cin >> message;

	if (send(sock, message, MsgSize, NULL) == SOCKET_ERROR)
		return false;

	// Получение ответа от сервера
	if (recv(sock, message, MsgSize, NULL) == SOCKET_ERROR)
		return false;

	// Отклонение подключения
	if (strcmp(message, "rejected") == 0)
		return false;

	cin.getline(message, MsgSize);
	Connected = true;
	return true;
}

bool IsConnected()
{
	return Connected;
}

void HandleIncomingMsg(SOCKET& sock)
{
	char message[MsgSize];

	while (Connected)
	{
		if (recv(sock, message, MsgSize, NULL) != SOCKET_ERROR)
		{
			cout << message << endl;
		}
		else
		{
			Mutex.lock();
			Connected = false;
			Mutex.unlock();
		}
	}
}

void HandleSendingMsg(SOCKET& sock)
{
	char message[MsgSize];
	cin.getline(message, MsgSize);

	if (send(sock, message, MsgSize, NULL) == SOCKET_ERROR || strcmp(message, "exit") == 0)
	{
		Mutex.lock();
		Connected = false;
		Mutex.unlock();
	}
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