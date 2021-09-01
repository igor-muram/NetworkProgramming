#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>
#include "Constants.h"
#include "PacketException.h"
#include "PacketType.h"

namespace Net
{
	class Packet
	{
	public:
		Packet(PacketType packetType = PacketType::Invalid);
		PacketType GetPacketType();
		void AssignPacketType(PacketType packetType);

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
