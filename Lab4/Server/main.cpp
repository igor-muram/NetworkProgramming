#include "WebServer.h"

int main()
{
	// ������������� �������
	SOCKET sock;
	InitServer(sock);

	while (true)
	{
		// ��������� �����������
		SOCKET clientSock = accept(sock, NULL, NULL);
		GetRequest(clientSock);
		closesocket(clientSock);
	}

	WSACleanup();
	system("pause");
	return 0;
}