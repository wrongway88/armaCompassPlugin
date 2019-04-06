#include <iostream>
#include <stdio.h>

#include <io.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <thread>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#define CURRENT_VERSION "1.0.0.0"

#define PORT "56172"

void updateCompass(float direction)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	WSAStartup(wVersionRequested, &wsaData);

	struct addrinfo* result = nullptr;
	struct addrinfo* ptr = nullptr;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int r = getaddrinfo("127.0.0.1", PORT, &hints, &result);

	if (r != 0)
	{
		WSACleanup();
		return;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	ptr = result;

	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (r == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}

	char tmp[64];
	sprintf_s(tmp, "%G<EOF>", direction);

	char* sendbuf = tmp;
	char recvbuf[512];

	r = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (r == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	r = shutdown(ConnectSocket, SD_SEND);
	closesocket(ConnectSocket);
	WSACleanup();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		// wtf?!
		break;
	}

	return true;
}

extern "C"
{
	__declspec (dllexport) void __stdcall RVExtensionVersion(char* output, int outputSize);
	__declspec (dllexport) void __stdcall RVExtension(char* output, int outputSize, const char* function);
	__declspec (dllexport) int __stdcall RVExtensionArgs(char* output, int outputSize, const char* function, const char** args, int argsCnt);
}

void __stdcall RVExtensionVersion(char* output, int outputSize)
{
	strncpy_s(output, outputSize, CURRENT_VERSION, _TRUNCATE);
}

void __stdcall RVExtension(char* output, int outputSize, const char* function)
{
	strncpy_s(output, outputSize, "void", _TRUNCATE);
}

int __stdcall RVExtensionArgs(char* output, int outputSize, const char* function, const char** args, int argsCnt)
{
	if (strcmp(function, "updateCompass") == 0)
	{
		if (argsCnt >= 1)
		{
			double direction = atof(args[0]);
			
			std::thread(updateCompass, direction).detach();

			strncpy_s(output, outputSize, "updating compass", _TRUNCATE);
		}

		return 100;
	}

	strncpy_s(output, outputSize, "void", _TRUNCATE);

	return 0;
}
