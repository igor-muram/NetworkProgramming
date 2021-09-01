#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>

namespace Net
{
	class Packet
	{
	public:
		void Clear();
		void Append(const void* data, uint32_t size);

		Packet& operator<<(uint32_t data);
		Packet& operator>>(uint32_t& data);

		Packet& operator<<(const std::string& data);
		Packet& operator>>(std::string& data);
		uint32_t extractionOffset = 0;
		std::vector<char> buffer;
	};
}
