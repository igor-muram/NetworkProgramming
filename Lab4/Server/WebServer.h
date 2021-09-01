#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;
const int BuffSize = 8192;

bool InitServer(SOCKET& sock);							// Инициализация сервера
addrinfo CreateAddress(string ip, string port);			// Создание адреса
bool GetRequest(SOCKET& sock);							// Получение запросов клиента
void HandleGetRequest(string& data, SOCKET& socket);	// Обработка GET запросов клиента
string GetFilename(const string data);					// Получение имени файла
string GetFileType(const string filename);				// Получение расширения файла 
string GetMediaType(const string filename);				// Получение типа документа
string ReadFile(string filename, int& length);			// Считывание файла и его длины