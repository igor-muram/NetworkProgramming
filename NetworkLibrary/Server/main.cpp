// Server code
#include <iostream>
#include <string>
#include <NetworkLibrary/IncludeMe.h>

bool ProcessPacket(Net::Packet& packet)
{
	switch (packet.GetPacketType())
	{
	case Net::PacketType::ChatMessage:
	{
		std::string chatMessage;
		packet >> chatMessage;
		std::cout << "Chat Message: " << chatMessage << std::endl;
		break;
	}
	case Net::PacketType::IntegerArray:
	{
		uint32_t arraySize = 0;
		packet >> arraySize;
		std::cout << "Array Size: " << arraySize << std::endl;
		for (int i = 0; i < arraySize; i++)
		{
			uint32_t element = 0;
			packet >> element;
			std::cout << "Element[" << i << "] - " << element << std::endl;
		}
		break;
	}
	default:
		return false;
	}

	return true;
}

int main()
{
	if (!Net::Network::Initialize())
	{
		std::cout << "Failed to initialize network." << std::endl;
		return 1;
	}

	Net::IPEndPoint test("0.0.0.0", 4790);
	Net::Socket socket;

	if (socket.Create() == Net::Result::Success)
		std::cout << "Socket created successfully." << std::endl;
	else
	{
		std::cout << "Failed to create socket." << std::endl;
		return 1;
	}

	if (socket.Listen(test) == Net::Result::Success)
		std::cout << "Socket successfully listening on port: " << test.GetPort() << std::endl;
	else
	{
		std::cout << "Socket failed to listen on port: " << test.GetPort() << std::endl;
		return 1;
	}

	Net::Socket newConnection;
	if (socket.Accept(newConnection) == Net::Result::Success)
	{
		std::cout << "New connection accepted." << std::endl;
	}
	else
		std::cout << "Failed to accept new connection." << std::endl;

	Net::Packet packet;

	Net::Result result = Net::Result::Success;
	while (result == Net::Result::Success)
	{
		result = newConnection.Recv(packet);

		if (!ProcessPacket(packet))
			result = Net::Result::GenericError;
	}

	newConnection.Close();
	Net::Network::Shutdown();

	system("pause");
	return 0;
}