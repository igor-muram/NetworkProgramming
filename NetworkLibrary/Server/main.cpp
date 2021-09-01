// Server code
#include <iostream>
#include <NetworkLibrary/IncludeMe.h>

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
	{
		std::cout << "Socket successfully listening on port: " << test.GetPort() << std::endl;
	}
	else
	{
		std::cout << "Socket failed to listen on port: " << test.GetPort() << std::endl;
		return 1;
	}

	Net::Network::Shutdown();

	system("pause");
	return 0;
}