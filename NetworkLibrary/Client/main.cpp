// Client code
#include <iostream>
#include <NetworkLibrary/IncludeMe.h>

int main()
{
	if (!Net::Network::Initialize())
	{
		std::cout << "Failed to initialize network." << std::endl;
		return 1;
	}

	Net::Socket socket;

	if (socket.Create() == Net::Result::Success)
		std::cout << "Socket created successfully." << std::endl;
	else
	{
		std::cout << "Failed to create socket." << std::endl;
		return 1;
	}

	if (socket.Connect(Net::IPEndPoint("127.0.0.1", 4790)) == Net::Result::Success)
		std::cout << "Successfully connected to server." << std::endl;
	else
		std::cout << "Failed to connect to server." << std::endl;


	Net::Packet stringPacket(Net::PacketType::ChatMessage);
	stringPacket << "This is my string packet.";

	Net::Packet integersPacket(Net::PacketType::IntegerArray);
	uint32_t arraySize = 6;
	uint32_t integerArray[6] = { 2, 5, 7, 1, 3, 5 };
	integersPacket << arraySize;
	for (auto integer : integerArray)
		integersPacket << integer;
		
	Net::Result result = Net::Result::Success;
	while (result == Net::Result::Success)
	{
		if (rand() % 2 == 0)
			result = socket.Send(stringPacket);
		else
			result = socket.Send(integersPacket);

		Sleep(2000);
	}

	Net::Network::Shutdown();

	system("pause");
	return 0;
}