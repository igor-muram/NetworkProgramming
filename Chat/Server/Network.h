#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <utility>
#include <list>
#include <cstring>
#include <algorithm>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

const int MaxConnections = 5;
const int MsgSize = 8192;

struct Client
{
	string name;
	int ID;
	bool isConnected = false;
	SOCKET sock;
	sockaddr_in addr;

};

inline bool operator ==(const Client& client1, const Client& client2)
{
	return client1.ID == client2.ID;
}

bool InitServer(SOCKET& sock, sockaddr& addr);			// Запуск сервера
int GetClientCount();									// Получить количество клиентов
void AcceptClient(Client& client);						// Присоединение клиента
void RejectClient(Client& client);						// Отклонение на присоединение клиента
void HandleClient(Client client);						// Обработка клиента
void HandleClientMessage(Client& client, string msg);	// Обработка сообщения клиента
void ClientDisconnect(Client& client);					// Отключение клиента от сервера
sockaddr CreateAddress(std::string ip, uint16_t port);	// Создание адреса
