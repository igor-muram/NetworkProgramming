#include "Network.h"

int ClientIDCount = 0;
mutex Mutex;
list<Client> clients;

bool InitServer(SOCKET& sock, sockaddr& addr)
{
	// Инициализация библиотке
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed to initialize library.\n";
		return false;
	}

	// Создание сокета
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cout << "Failed to create socket.\n";
		return false;
	}

	// Привязка сокета к адресу
	if (bind(sock, &addr, sizeof(sockaddr)) != 0)
	{
		cout << "Failed to bind socket.\n";
		return false;
	}

	// Установка сокета на прослушивание порта
	if (listen(sock, MaxConnections) != 0)
	{
		cout << "Failed to listen.\n";
		return false;
	}

	return true;

}

int GetClientCount()
{
	return clients.size();
}

void AcceptClient(Client& client)
{
	// Получение данных о клиенте
	char name[MsgSize];
	recv(client.sock, name, MsgSize, NULL);
	client.name = name;
	client.ID = ClientIDCount++;
	client.isConnected = true;
	clients.push_back(client);

	// Оповещение клиента о успешном подключении
	send(client.sock, "accepted", 9, NULL);

	// Вывод имени и адреса клиента
	char clientIP[16];
	inet_ntop(AF_INET, &client.addr.sin_addr, clientIP, 16);
	cout << client.name << " has been connected.\n";
	cout << "IP address: " << clientIP << endl;

	// Оповещение других клиентов о новом клиенте
	string tempMsg = client.name + " joined chat. IP: " + clientIP;
	for (auto x : clients)
		if (client.ID != x.ID)
			send(x.sock, tempMsg.c_str(), MsgSize, NULL);

	cout << "Current amount of clients: " << GetClientCount() << endl << endl;

	// Создание отдельного потока для обработки клиента
	new thread(HandleClient, client);
}

void RejectClient(Client& client)
{
	send(client.sock, "rejected", 9, NULL);
	closesocket(client.sock);
}

void HandleClient(Client client)
{
	char message[MsgSize];
	std::string tempMsg;

	while (client.isConnected)
	{
		if (recv(client.sock, message, MsgSize, NULL) != SOCKET_ERROR)
			HandleClientMessage(client, message);
		else
			client.isConnected = false;
	}

	ClientDisconnect(client);
}

void HandleClientMessage(Client& client, string msg)
{
	// Проверка на команду выхода
	if (msg == "exit")
	{
		client.isConnected = false;
		return;
	}
	
	// Вывод сообщения на сервере
	msg = client.name + ": " + msg;
	Mutex.lock();
	cout << msg << endl;
	Mutex.unlock();

	// Рассылка сообщения всем остальным клиентам
	for (auto x : clients)
		if (client.ID != x.ID)
			send(x.sock, msg.c_str(), MsgSize, NULL);

}

void ClientDisconnect(Client& client)
{
	// Удаление клиента из списка
	cout << client.name << " left the chat.\n";

	clients.remove(client);

	// Оповещение других клиентов о выходе
	string msg = client.name + " left the chat.";
	for (auto x : clients)
		send(x.sock, msg.c_str(), MsgSize, NULL);

	closesocket(client.sock);
}

sockaddr CreateAddress(string ip, uint16_t port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	return *reinterpret_cast<sockaddr*>(&addr);
}
