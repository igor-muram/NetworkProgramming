#include "Network.h"

int main()
{
	// ������������� ����������
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		HandleError("Failed to initialize library.");

	// �������� ������
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// ����������� � �������
	string IP;
	cout << "IP > ";
	cin >> IP;
	sockaddr addr = CreateAddress(IP, 2005);

	bool result = ConnectToServer(sock, addr);
	if (result)
	{
		// ��������� �������� � ��������� ���������
		thread thread(HandleIncomingMsg, std::ref(sock));

		while (IsConnected())
			HandleSendingMsg(sock);

		thread.join();
	}
	else
		cout << "Connection to server rejected. Too many clients.\n";

	cout << WSAGetLastError();
		
	closesocket(sock);
	WSACleanup();
	return 0;
}
