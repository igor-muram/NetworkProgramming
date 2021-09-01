#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <WS2tcpip.h>
#include "IPVersion.h"

namespace Net
{
	class IPEndPoint
	{
	public:
		IPEndPoint(const char* ip, unsigned short port);
		IPEndPoint(sockaddr* addr);
		IPVersion GetIPVersion();
		std::vector<uint8_t> GetIPBytes();
		std::string GetHostname();
		std::string GetIPString();
		unsigned short GetPort();
		sockaddr_in GetSockaddrIPv4();
		void Print();

	private:
		IPVersion ipversion = IPVersion::Unknown;
		std::string hostname = "";
		std::string ip_string = "";
		std::vector<uint8_t> ip_bytes;
		unsigned short port = 0;
	};
}