#include "Network.h"

bool Net::Network::Initialize()
{
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (result != 0)
	{
		std::cout << "ERROR. Failed to start up the library" << std::endl;
		return false;
	}
	
	return true;
}

void Net::Network::Shutdown()
{
	WSACleanup();
}
