#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <iostream>
#include <string>

using namespace std;

const int BufferSize = 4;
const int requestCount = 4;
const char TTL = 64;

void InitLibrary();
SOCKET InitSock();
bool ConvertHostName(string& hostname, string& host, IPAddr& ipaddr);	
void MakeRequest(IPAddr ipaddr, string& host);