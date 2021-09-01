#include "WebServer.h"

bool InitServer(SOCKET& sock)
{
	// Инициализация библиотеки
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Failed to initialize library.\n";
		return false;
	}

	// Создание адреса
	addrinfo addr = CreateAddress("0.0.0.0", "2005");

	// Создание сокета
	sock = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		cout << "Failed to create socket.\n";
		return false;
	}

	// Привязка сокета к адресу
	if (bind(sock, addr.ai_addr, addr.ai_addrlen) != 0)
	{
		cout << "Failed to bind socket to address.\n";
		return false;
	}

	// Установка сокета на прослушивание порта
	if (listen(sock, SOMAXCONN) != 0)
	{
		cout << "Failed to listen to address.\n";
		return false;
	}

	return true;
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

bool GetRequest(SOCKET& sock)
{
	// Получение данных
	char data[BuffSize];
	int result = recv(sock, data, BuffSize, NULL);
	if (result == SOCKET_ERROR)
		return false;

	data[result] = '\0';
	// Вывод данных на сервере
	cout << "Client request:\n" << data << endl;

	string tempData(data);
	// Обработка метода GET
	if (tempData.find("GET", 0) == 0)
		HandleGetRequest(tempData, sock);

}

void HandleGetRequest(string& data, SOCKET& socket)
{
	// Получение имени файла из данных
	string filename = GetFilename(data);
	if (filename == "")
		filename = "index.html";

	// Получение содержимого файла и его длины
	int length = 0;
	string body = ReadFile(filename, length);
	string mediatype = GetMediaType(filename);

	stringstream response;
	// Составление ответа для клиента
	if (body != "" && mediatype != "")
	{
		response
			<< "HTTP/1.1 200 OK\r\n"
			<< "Server: ninja\r\n"
			<< "Content-Type: " << GetMediaType(filename)
			<< "\r\n"
			<< "Content-Length: " << length
			<< "\r\n\r\n";
	}
	else
	{
		response << "HTTP/1.1 404 File not found\r\n\r\n";
		body = "<h1 style=\"text-align: center;\">404 - File not found</h1>";
	}

	cout << "Server response:\n" << response.str();
	response << body;

	// Отправка ответа клиенту
	send(socket, response.str().c_str(), response.str().length(), NULL);
}

string GetFilename(const string data)
{
	string temp = data.substr(0, data.find_first_of('\r'));
	int start = temp.find_first_of('/') + 1;
	int end = temp.find_last_of(' ');
	string filename = temp.substr(start, end - start);
	return filename;
}

string GetFileType(const string filename)
{
	int start = filename.find_last_of('.') + 1;
	string type = filename.substr(start, filename.length() - start);
	return type;
}

string GetMediaType(const string filename)
{
	string filetype = GetFileType(filename);

	if (filetype == "html" || filename == "")
		return "text/html";

	if (filetype == "css")
		return "text/css";

	if (filetype == "js")
		return "application/javascript";

	if (filetype == "pdf")
		return "application/pdf";

	if (filetype == "ttf")
		return "application/x-font-ttf";

	if (filetype == "woff")
		return "application/font-woff";

	if (filetype == "woff2")
		return "application/font-woff2";

	return "";
}

string ReadFile(string filename, int& length)
{
	ifstream in(filename, ios::in | ios::binary);

	if (in)
	{
		in.seekg(0, in.end);
		length = in.tellg();
		in.seekg(0, in.beg);

		string body;
		body.resize(length);
		in.read(&body[0], streamsize(length));
		in.close();

		return body;
	}
	else
	{
		length = 0;
		return "";
	}
}
