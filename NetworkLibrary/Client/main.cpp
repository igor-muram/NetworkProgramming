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


	Net::Packet packet;
	packet << "String1";
	packet << "String2";
		
	Net::Result result = Net::Result::Success;
	while (result == Net::Result::Success)
	{
		result = socket.Send(packet);
		Sleep(2000);
	}

	Net::Network::Shutdown();

	system("pause");
	return 0;
}