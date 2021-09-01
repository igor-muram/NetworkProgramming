#include "Network.h"

int main()
{
	// Инициализация библиотеки
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed to initialize library.\n";
		return -1;
	}

	// Создание сокета
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Подключение к серверу
	string IP;
	cout << "IP > ";
	cin >> IP;
	sockaddr addr = CreateAddress(IP, 2005);

	bool result = ConnectToServer(sock, addr);
	if (result)
	{
		// Обработка входящих и исходящих сообщений
		thread thread(HandleIncomingMsg, std::ref(sock));

		while (IsConnected())
			HandleSendingMsg(sock);

		thread.join();
	}
	else
		cout << "Connection to server rejected. Too many clients.\n";
		
	closesocket(sock);
	WSACleanup();
	system("pause");
	return 0;
}
