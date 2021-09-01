#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;
const int BuffSize = 8192;

addrinfo CreateAddress(string ip, string port);
void HandleError(string message);
void HandleGetRequest(string data, SOCKET socket);

int main()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		HandleError("Failed to initialize library.");

	addrinfo addr = CreateAddress("0.0.0.0", "2005");

	SOCKET listenSock = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
	if (listenSock == INVALID_SOCKET)
		HandleError("Failed to create socket.");

	if (bind(listenSock, addr.ai_addr, addr.ai_addrlen) != 0)
		HandleError("Failed to bind socket.");

	if (listen(listenSock, SOMAXCONN) != 0)
		HandleError("Failed to listen.");

	while (true)
	{
		SOCKET clientSock = accept(listenSock, NULL, NULL);

		char data[BuffSize];
		int result = recv(clientSock, data, BuffSize, NULL);
		if (result == SOCKET_ERROR)
			HandleError("Failed to receive data from client.");

		data[result] = '\0';
		std::cout << "Client request:\n" << data << std::endl;

		string tempData(data);

		if (tempData.find("GET", 0) == 0)
			HandleGetRequest(tempData, clientSock);

		closesocket(clientSock);
	}

	freeaddrinfo(&addr);
	WSACleanup();
	system("pause");
	return 0;
}

addrinfo CreateAddress(string ip, string port)
{
	addrinfo* addr = nullptr;
	addrinfo hints = { };

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(ip.c_str(), port.c_str(), &hints, &addr);

	return *addr;
}

void HandleError(string message)
{
	cout << "ERROR. " << message << endl;
	system("pause");
	exit(1);
}

void HandleGetRequest(string data, SOCKET socket)
{
	string filename = data.substr(5, data.find_first_of(' ', 5) - 5);

	stringstream response;
	string responseBody;

	response
		<< "HTTP/1.1 200 OK\r\n"
		<< "Server: ninja\r\n"
		<< "Content-Type: text/html\r\n";

	if (filename == "")
	{
		ifstream in("index.html");

		in.seekg(0, in.end);
		int filelength = in.tellg();
		in.seekg(0, in.beg);

		response
			<< "Content-Length: " << filelength
			<< "\r\n\r\n";

		responseBody.resize(filelength, ' ');
		in.read(&responseBody[0], filelength);
	}
	else
	{
		ifstream in(filename);

		in.seekg(0, in.end);
		int filelength = in.tellg();
		in.seekg(0, in.beg);

		if (!in.fail())
		{
			cout << "File " << filename << " found" << endl << endl;

			response
				<< "Content-Length: " << filelength
				<< "\r\n\r\n";

			responseBody.resize(filelength, ' ');
			in.read(&responseBody[0], filelength);
		}
		else
		{
			cout << "File " << filename << " not found" << endl << endl;
			response << "HTTP/1.1 404 File not found\r\n\r\n";
			responseBody = "<h1 style=\"text-align: center;\">404 - File not found</h1>";
		}
	}

	send(socket, response.str().c_str(), response.str().length(), NULL);
	send(socket, responseBody.c_str(), responseBody.length(), NULL);
}