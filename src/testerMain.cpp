#include <iostream>

#include <Windows.h>

typedef void(__stdcall *RVExtension)(char*, int, const char*);
typedef void(__stdcall *RVExtensionArgs)(char*, int, const char*, const char**, int);

int main()
{
	HMODULE moduleHandle = LoadLibraryA("CompassClient_x64.dll");

	if (moduleHandle != nullptr)
	{
		RVExtension rvExtension = (RVExtension)GetProcAddress(moduleHandle, "RVExtension");
		RVExtensionArgs rvExtensionArgs = (RVExtensionArgs)GetProcAddress(moduleHandle, "RVExtensionArgs");

		char buffer[512];

		const char* args[1] = {"180"};
		
		// Verify success by monitoring port 56172
		// You should observe a message like "180<EOF>"
		if (rvExtensionArgs)
		{
			rvExtensionArgs(buffer, 512, "updateCompass", args, 1);
		}
	}

	return 0;
}