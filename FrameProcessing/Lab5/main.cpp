#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>

using namespace std;

enum FrameType
{
	IPv4,
	DIX,
	Novell,
	SNAP,
	LLC
};

int ReadFile(string filename, char** data)
{
	ifstream in(filename, ios::in | ios::binary);
	if (in.fail())
		return -1;

	in.seekg(0, ios_base::end);
	int length = in.tellg();
	in.seekg(0, ios_base::beg);

	*data = new char[length];
	in.read(*data, length);
	in.close();

	return length;
}

void WriteMAC(FILE* out, char* data)
{
	for (int i = 0; i < 5; i++)
		fprintf_s(out, "%02X:", (unsigned char)data[i]);

	fprintf_s(out, "%02X\n", (unsigned char)data[5]);
}

void WriteIP(FILE* out, char* data)
{
	for (int i = 0; i < 3; i++)
		fprintf_s(out, "%d.", (unsigned char)data[i]);

	fprintf_s(out, "%d\n", (unsigned char)data[3]);
}

int GetFrameType(char* data)
{
	u_short L = ntohs(*(u_short*)data);
	if (L == 0x800)
		return IPv4;

	if (L > 1500)
		return DIX;

	data += 2;
	L = ntohs(*(u_short*)data);
	if (L == 0xFFFF)
		return Novell;

	if (L == 0xAAAA)
		return SNAP;

	return LLC;
}

void Parse(char* data, int length)
{
	int FrameCount = 0;
	int IPv4FrameCount = 0;
	int DIXFrameCount = 0;
	int NovellFrameCount = 0;
	int SNAPFrameCount = 0;
	int LLCFrameCount = 0;

	FILE* out = nullptr;
	fopen_s(&out, "result.txt", "w");

	fprintf_s(out, "size of file: %d bytes\n\n", length);
	char* iter = data;
	while (iter < data + length)
	{
		FrameCount++;
		fprintf_s(out, "frame ¹: %d\n", FrameCount);
		fprintf_s(out, "Destination MAC: ");
		WriteMAC(out, iter);
		iter += 6;

		fprintf_s(out, "Source MAC:      ");
		WriteMAC(out, iter);
		iter += 6;

		u_short L = ntohs(*(u_short*)(iter));
		int type = GetFrameType(iter);
		iter += 2;

		fprintf_s(out, "Frame type:      ");
		switch (type)
		{
		case IPv4:
			fprintf_s(out, "IPv4\n");
			fprintf_s(out, "Source IP:       ");
			WriteIP(out, iter + 12);
			fprintf_s(out, "Destination IP:  ");
			WriteIP(out, iter + 16);
			L = ntohs(*(u_short*)(iter + 2));
			iter += L;
			IPv4FrameCount++;
			break;
		case DIX:
			fprintf_s(out, "Ethernet DIX (Ethernet II)\n");
			iter += L;
			DIXFrameCount++;
			break;
		case Novell:
			fprintf_s(out, "Raw 802.3 (Novell 802.3)\n");
			iter += L;
			NovellFrameCount++;
			break;
		case SNAP:
			fprintf_s(out, "Ethernet SNAP\n");
			iter += L;
			SNAPFrameCount++;
			break;
		case LLC:
			fprintf_s(out, "802.3/LLC\n");
			iter += L;
			LLCFrameCount++;
			break;
		}

		fprintf_s(out, "%s\n\n", string(34, '-').c_str());
	}

	fprintf_s(out, "Frames:                            %d\n", FrameCount);
	fprintf_s(out, "IPv4 frames:                       %d\n", IPv4FrameCount);
	fprintf_s(out, "Ethernet DIX (Ethernet II) frames: %d\n", DIXFrameCount);
	fprintf_s(out, "Raw 802.3 (Novell 802.3) frames:   %d\n", NovellFrameCount);
	fprintf_s(out, "Ethernet SNAP frames:              %d\n", SNAPFrameCount);
	fprintf_s(out, "802.3/LLC frames:                  %d\n", LLCFrameCount);
}

int main()
{
	string filename;
	cout << "Enter filename: ";
	cin >> filename;
	char* data = nullptr;
	int length = ReadFile(filename, &data);

	if (length != -1)
		Parse(data, length);
	else
		cout << "ERROR! Failed to read file.";

	return 0;
}