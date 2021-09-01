#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <mutex>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

const int MsgSize = 8192;

bool ConnectToServer(SOCKET& sock, sockaddr& addr);		// Подключение к серверу
bool IsConnected();										// Проверка подключения к серверу
void HandleIncomingMsg(SOCKET& sock);					// Обработка сообщений, приходящих с сервера
void HandleSendingMsg(SOCKET& sock);					// Отправка сообщений
sockaddr CreateAddress(string ip, uint16_t port);		// Создание адреса
