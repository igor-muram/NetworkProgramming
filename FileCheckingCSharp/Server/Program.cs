using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Server
{
	class Program
	{
		static void Main(string[] args)
		{
			Socket listenSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			Console.Write("IP > ");
			IPAddress ip = IPAddress.Parse(Console.ReadLine());
			Console.Write("Port > ");
			int port = int.Parse(Console.ReadLine());
			IPEndPoint address = new IPEndPoint(ip, port);
			listenSock.Bind(address);
			listenSock.Listen(10);

			while(true)
			{
				Socket clientSock = listenSock.Accept();
				byte[] data = new byte[256];
				clientSock.Receive(data);
				Console.WriteLine(Encoding.ASCII.GetString(data));
				string hello = "Hello, Client!";
				data = Encoding.ASCII.GetBytes(hello);
				clientSock.Send(data);

				clientSock.Close();
			}
		}
	}
}
