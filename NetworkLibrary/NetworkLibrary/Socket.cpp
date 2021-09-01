#include "Socket.h"

namespace Net
{
	Net::Socket::Socket(IPVersion ipversion, SocketHandle handle)
		: ipversion(ipversion), handle(handle)
	{
		assert(ipversion == IPVersion::IPv4);
	}

	Result Net::Socket::Create()
	{
		assert(ipversion == IPVersion::IPv4);

		if (handle != INVALID_SOCKET)
			return Result::NotYetImplemented;

		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (handle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != Result::Success)
			return Result::NotYetImplemented;

		return Result::Success;
	}

	Result Net::Socket::Close()
	{
		if (handle == INVALID_SOCKET)
			return Result::NotYetImplemented;

		int result = closesocket(handle);

		if (result != 0)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		handle = INVALID_SOCKET;
		return Result::Success;
	}

	Result Socket::Bind(IPEndPoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = bind(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
		if (result != 0)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		return Result::Success;
	}

	Result Socket::Listen(IPEndPoint endpoint, int backlog)
	{
		if (Bind(endpoint) != Result::Success)
			return Result::NotYetImplemented;

		int result = listen(handle, backlog);

		if (result != 0)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		return Result::Success;
	}

	Result Socket::Accept(Socket& outSocket)
	{
		sockaddr_in addr = {};
		int len = sizeof(sockaddr_in);

		SocketHandle acceptedConnectionHandle = accept(handle, reinterpret_cast<sockaddr*>(&addr), &len);
		if (acceptedConnectionHandle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}
		IPEndPoint newConnectionEndPoint(reinterpret_cast<sockaddr*>(&addr));
		std::cout << "New connection accepted!" << std::endl;
		newConnectionEndPoint.Print();

		outSocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);
		return Result::Success;
	}

	Result Socket::Connect(IPEndPoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();

		int result = connect(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));

		if (result != 0)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		return Result::Success;
	}

	Result Socket::Send(void* data, int numberOfBytes, int& bytesSent)
	{
		bytesSent = send(handle, reinterpret_cast<const char*>(data), numberOfBytes, NULL);

		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		return Result::Success;
	}

	Result Socket::Recv(void* destination, int numberOfBytes, int& bytesReceived)
	{
		bytesReceived = recv(handle, reinterpret_cast<char*>(destination), numberOfBytes, NULL);

		if (bytesReceived == 0)
			return Result::NotYetImplemented;

		if (bytesReceived == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		return Result::Success;
	}

	SocketHandle Net::Socket::GetHandle()
	{
		return handle;
	}

	IPVersion Net::Socket::GetIPVersion()
	{
		return ipversion;
	}

	Result Socket::SetSocketOption(SocketOption option, BOOL value)
	{
		int result = 0; 

		switch (option)
		{
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&value), sizeof(value));
			break;
		default:
			return Result::NotYetImplemented;
		}

		if (result != 0)
		{
			int error = WSAGetLastError();
			return Result::NotYetImplemented;
		}

		return Result::Success;
	}
}