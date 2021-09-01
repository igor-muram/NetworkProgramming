#include "IPEndPoint.h"

namespace Net
{
	Net::IPEndPoint::IPEndPoint(const char* ip, unsigned short port)
		: port(port)
	{
		in_addr addr;
		int result = inet_pton(AF_INET, ip, &addr);

		if (result == 1 &&addr.S_un.S_addr != INADDR_NONE)
		{
			ip_string = ip;
			hostname = ip;

			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));

			ipversion = IPVersion::IPv4;
			return;
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

		in6_addr addr6;
		result = inet_pton(AF_INET6, ip, &addr6);

		if (result == 1 && addr.S_un.S_addr != INADDR_NONE)
		{
			ip_string = ip;
			hostname = ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addr6.u, 16);

			ipversion = IPVersion::IPv6;
			return;
		}

		addrinfo hintsv6 = {};
		hintsv6.ai_family = AF_INET6;
		addrinfo* hostinfov6 = nullptr;
		result = getaddrinfo(ip, NULL, &hintsv6, &hostinfov6);
		if (result == 0)
		{

			sockaddr_in6* host_addr = reinterpret_cast<sockaddr_in6*>(hostinfov6->ai_addr);

			ip_string.resize(46);
			inet_ntop(AF_INET6, &host_addr->sin6_addr, &ip_string[0], 46);

			hostname = ip;

			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &host_addr->sin6_addr, 16);

			ipversion = IPVersion::IPv6;

			freeaddrinfo(hostinfov6);
			return;
		}

	}

	IPEndPoint::IPEndPoint(sockaddr* addr)
	{
		assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);

		if (addr->sa_family == AF_INET)
		{
			sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
			ipversion = IPVersion::IPv4;
			port = ntohs(addrv4->sin_port);
			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &addrv4->sin_addr, sizeof(ULONG));
			ip_string.resize(16);
			inet_ntop(AF_INET, &addrv4->sin_addr, &ip_string[0], 16);
			hostname = ip_string;
		}
		else
		{
			sockaddr_in6* addrv6 = reinterpret_cast<sockaddr_in6*>(addr);
			ipversion = IPVersion::IPv6;
			port = ntohs(addrv6->sin6_port);
			ip_bytes.resize(16);
			memcpy(&ip_bytes[0], &addrv6->sin6_addr, 16);
			ip_string.resize(46);
			inet_ntop(AF_INET6, &addrv6->sin6_addr, &ip_string[0], 46);
			hostname = ip_string;
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

	sockaddr_in6 IPEndPoint::GetSockaddrIPv6()
	{
		assert(ipversion == IPVersion::IPv6);
		sockaddr_in6 addr = {};
		addr.sin6_family = AF_INET6;
		memcpy(&addr.sin6_addr, &ip_bytes[0], 16);
		addr.sin6_port = htons(port);
		return addr;
	}

	void IPEndPoint::Print()
	{
		switch (ipversion)
		{
		case IPVersion::IPv4:
			std::cout << "IP Version: IPv4" << std::endl;
			break;
		case IPVersion::IPv6:
			std::cout << "IP Version: IPv6" << std::endl;
			break;
		default:
			std::cout << "IP Version: Unknown" << std::endl;
		}

		std::cout << "Hostname: " << hostname << std::endl;
		std::cout << "IP: " << ip_string << std::endl;
		std::cout << "Port: " << port << std::endl;
		std::cout << "IP bytes..." << std::endl;
		for (auto& digit : ip_bytes)
		{
			std::cout << (int)digit << std::endl;
		}
	}
}