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

bool ConnectToServer(SOCKET& sock, sockaddr& addr);		// ����������� � �������
bool IsConnected();										// �������� ����������� � �������
void HandleIncomingMsg(SOCKET& sock);					// ��������� ���������, ���������� � �������
void HandleSendingMsg(SOCKET& sock);					// �������� ���������
sockaddr CreateAddress(string ip, uint16_t port);		// �������� ������
