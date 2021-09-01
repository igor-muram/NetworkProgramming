#include "IPEndPoint.h"

namespace Net
{
	Net::IPEndPoint::IPEndPoint(const char* ip, unsigned short port)
		: port(port)
	{
		in_addr addr;
		int result = inet_pton(AF_INET, ip, &addr);

		if (result == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				ip_string = ip;
				hostname = ip;

				ip_bytes.resize(sizeof(ULONG));
				memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));

				ipversion = IPVersion::IPv4;
				return;
			}
		}

		addrinfo hints = {};
		hints.ai_family = AF_INET;
		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip, NULL, &hints, &hostinfo);
		if (result == 0)
		{
			sockaddr_in* host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);

			ip_string.resize(16);
			inet_ntop(AF_INET, &host_addr->sin_addr, &ip_string[0], 16);

			hostname = ip;

			ULONG ip_long = host_addr->sin_addr.S_un.S_addr;
			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &ip_long, sizeof(ULONG));

			ipversion = IPVersion::IPv4;

			freeaddrinfo(hostinfo);
			return;
		}

	}

	IPVersion Net::IPEndPoint::GetIPVersion()
	{
		return ipversion;
	}

	std::vector<uint8_t> Net::IPEndPoint::GetIPBytes()
	{
		return ip_bytes;
	}

	std::string Net::IPEndPoint::GetHostname()
	{
		return hostname;
	}

	std::string Net::IPEndPoint::GetIPString()
	{
		return ip_string;
	}

	unsigned short Net::IPEndPoint::GetPort()
	{
		return port;
	}

	sockaddr_in IPEndPoint::GetSockaddrIPv4()
	{
		assert(ipversion == IPVersion::IPv4);

		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr, &ip_bytes[0], sizeof(ULONG));
		addr.sin_port = htons(port);

		return addr;
	}
}