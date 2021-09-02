using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Client
{
	class Program
	{
		static void Main(string[] args)
		{
			Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			Console.Write("IP > ");
			IPAddress ip = IPAddress.Parse(Console.ReadLine());
			Console.Write("Port > ");
			int port = int.Parse(Console.ReadLine());
			IPEndPoint address = new IPEndPoint(ip, port);
			sock.Connect(address);

			string hello = "Hello, Server!";
			sock.Send(Encoding.ASCII.GetBytes(hello));
			byte[] data = new byte[256];
			sock.Receive(data);

			Console.WriteLine(Encoding.ASCII.GetString(data));

			Console.ReadKey();
		}
	}
}
