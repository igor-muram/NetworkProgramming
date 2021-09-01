#pragma once
#include <iostream>
#include <assert.h>
#include "SocketHandle.h"
#include "Result.h"
#include "IPVersion.h"
#include "SocketOptions.h"
#include "IPEndPoint.h"
#include "Constants.h"
#include "Packet.h"

namespace Net
{
	class Socket
	{
	public:
		Socket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
		Result Create();
		Result Close();
		Result Bind(IPEndPoint endpoint);
		Result Listen(IPEndPoint endpoint, int backlog = 5);
		Result Accept(Socket &outSocket);
		Result Connect(IPEndPoint endpoint);
		Result Send(const void* data, int numberOfBytes, int& bytesSent);
		Result Recv(void* destination, int numberOfBytes, int& bytesReceived);
		Result SendAll(const void* data, int numberOfBytes);
		Result RecvAll(void* destination, int numberOfBytes);
		Result Send(Packet& packet);
		Result Recv(Packet& packet);
		SocketHandle GetHandle();
		IPVersion GetIPVersion();

	private:
		IPVersion ipversion = IPVersion::IPv4;
		SocketHandle handle = INVALID_SOCKET;

		Result SetSocketOption(SocketOption option, BOOL value);
	};
}

