#include <iostream>
#include <string>

#include "Network.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc, char** argv)
{
	// Check count of agruments
	if (argc != 2)
	{
		cout << "ERROR! Missing arguments." << endl;
		return -1;
	}

	// Initialize library
	InitLibrary();

	// Initialize socket
	SOCKET sock = InitSock();

	// Convert hostname to host ip address
	string hostname = argv[1];
	string host;
	IPAddr ipaddr;
	ConvertHostName(hostname, host, ipaddr);

	cout 
		<< "\n\nPinging with " 
		<< hostname
		<< " [" << host << "] " 
		<< "with " 
		<< BufferSize + sizeof(ICMP_ECHO_REPLY) 
		<< " bytes of data:\n";

	MakeRequest(ipaddr, host);
}