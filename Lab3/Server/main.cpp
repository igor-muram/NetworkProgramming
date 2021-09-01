#include "Network.h"

int main()
{
	SOCKET listenSocket;

	string IP;
	cout << "IP > ";
	cin >> IP;
	sockaddr addr = CreateAddress(IP, 2005);

	// Запуск сервера
	bool result = InitServer(listenSocket, addr);

	if (true)
	{
		cout << "Server launched.\n";
		cout << "Listen to the port 2005. Waiting for connections.\n";

		while (true)
		{
			// Обработка подключений клиентов
			Client client = {};
			int len = sizeof(SOCKADDR_IN);
			client.sock = accept(listenSocket, reinterpret_cast<sockaddr*>(&client.addr), &len);

			if (GetClientCount() < MaxConnections)
				AcceptClient(client);
			else
				RejectClient(client);
		}
	}

	system("pause");
	WSACleanup();
	return 0;
}
