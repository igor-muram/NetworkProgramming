#pragma once
#include <string>
#include <WS2tcpip.h>

struct Client
{
	std::string name;
	sockaddr addr;
};

void HandleClient()
{

}