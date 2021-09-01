#pragma once

#include <WinSock2.h>
#include <iostream>

namespace Net 
{
	class Network
	{
	public:
		static bool Initialize();
		static void Shutdown();
	};

}