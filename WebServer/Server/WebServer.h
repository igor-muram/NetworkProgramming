#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;
const int BuffSize = 8192;

bool InitServer(SOCKET& sock);							// ������������� �������
addrinfo CreateAddress(string ip, string port);			// �������� ������
bool GetRequest(SOCKET& sock);							// ��������� �������� �������
void HandleGetRequest(string& data, SOCKET& socket);	// ��������� GET �������� �������
string GetFilename(const string data);					// ��������� ����� �����
string GetFileType(const string filename);				// ��������� ���������� ����� 
string GetMediaType(const string filename);				// ��������� ���� ���������
string ReadFile(string filename, int& length);			// ���������� ����� � ��� �����