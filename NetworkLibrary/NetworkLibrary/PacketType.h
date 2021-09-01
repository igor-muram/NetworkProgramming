#pragma once
#include <cstdint>

namespace Net
{
	enum PacketType : uint16_t
	{
		Invalid,
		ChatMessage,
		IntegerArray
	};
}