#include "WebServer.h"

int main()
{
	// Инициализация сервера
	SOCKET sock;
	InitServer(sock);

	while (true)
	{
		// Обработка подключений
		SOCKET clientSock = accept(sock, NULL, NULL);
		GetRequest(clientSock);
		closesocket(clientSock);
	}

	WSACleanup();
	system("pause");
	return 0;
}