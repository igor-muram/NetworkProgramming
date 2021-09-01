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
	{
		std::cout << "Socket created successfully." << std::endl;
		socket.Close();
	}
	else
	{
		std::cout << "Failed to create socket." << std::endl;
		return 1;
	}

	Net::Network::Shutdown();

	system("pause");
	return 0;
}