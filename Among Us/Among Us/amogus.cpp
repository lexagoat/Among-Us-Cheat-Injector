#include <Windows.h>
#include <string>
#include <conio.h>
#include <iostream>

std::string chooseDLL()
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	if (GetOpenFileName(&ofn))
		return fileName;
}

int main()
{
	std::cout << "Lexa's 'Among Us' DLL Injector. Press enter to choose DLL" << std::endl;
	_getch();

	std::string stringpath = chooseDLL();
	const char* dllPath = stringpath.c_str();
	std::cout << "You choose " << dllPath << std::endl;
	Sleep(1000);
	std::cout << "Press Enter To Inject" << std::endl;
	_getch();

	DWORD pID;
	GetWindowThreadProcessId(FindWindow(NULL, "Among Us"), &pID);

	if (!pID)
	{
		MessageBoxA(NULL, "Couldn't Get Process ID. Check 'Among Us' Is Open", "Error", NULL);
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pID);

	if (hProcess == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Couldn't Get Handle to Among Us Check 'Among Us' Is Open", "Error", NULL);
	}

	void* allocmemory = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	WriteProcessMemory(hProcess, allocmemory, dllPath, MAX_PATH, 0);

	HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocmemory, 0, 0);

	if (hThread)
	{
		MessageBoxA(NULL, "Injected Successfully", "Success", NULL);
		VirtualFreeEx(hProcess, allocmemory, NULL, MEM_RELEASE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
	}

	return 0;
}