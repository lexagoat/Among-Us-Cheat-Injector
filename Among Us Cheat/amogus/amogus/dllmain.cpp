// dllmain.cpp : DLL uygulamasının giriş noktasını tanımlar.
#include "pch.h"
#include <Windows.h>
#include <vector>

DWORD FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
    unsigned int i = 0;

    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i);
    {
        Sleep(100);
        addr = *(uintptr_t*)addr;
        addr += offsets[i];
    }
    return addr;
}

DWORD WINAPI cheatthread(LPVOID param)
{
    std::vector<unsigned int> speedoffsets = { 0x5C, 0x4, 0x14 };

    DWORD modulebaseaddr = (DWORD)GetModuleHandleA("GameAssembly.dll");
    DWORD baseaddr = modulebaseaddr + 0x0014;

    uintptr_t speedaddr = FindDMAAddy(baseaddr, speedoffsets);

    *(float*)speedaddr = 10;

    MessageBoxA(NULL, "Injected Successfully", "Injected",NULL);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(nullptr, 0, cheatthread, nullptr, 0, nullptr);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
