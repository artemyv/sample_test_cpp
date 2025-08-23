// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE /*hModule*/,
                       DWORD  /*ul_reason_for_call*/,
                       LPVOID /*lpReserved*/
                     ) noexcept
{
    /* ul_reason_for_call could be one of the following:
        - DLL_PROCESS_ATTACH:
        - DLL_THREAD_ATTACH:
        - DLL_THREAD_DETACH:
        - DLL_PROCESS_DETACH:
        for now we do not need to handle any of these cases
    */
    return TRUE;
}
