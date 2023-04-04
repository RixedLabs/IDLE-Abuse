//Author : Navneet & ElementalX

#include <windows.h>
#include <iostream>
#include <winternl.h>
#include <TlHelp32.h>



DWORD MyWaitForInputIdleRoutine(HANDLE hProcess, DWORD dwMilliseconds)
{

    MessageBoxA(NULL, "Hello World", "POP POP", MB_OK);
}


typedef DWORD(WINAPI* WaitForInputIdleType)(HANDLE, DWORD);


WaitForInputIdleType UserWaitForInputIdleRoutine = NULL;


VOID WINAPI RegisterWaitForInputIdle(WaitForInputIdleType lpfnRegisterWaitForInputIdle)
{
    UserWaitForInputIdleRoutine = lpfnRegisterWaitForInputIdle;
}


UINT WINAPI MyWinExec(LPCSTR lpCmdLine, UINT uCmdShow)
{
    STARTUPINFOA StartupInfo;
    PROCESS_INFORMATION ProcessInformation;
    DWORD dosErr;

    RtlZeroMemory(&StartupInfo, sizeof(StartupInfo));
    StartupInfo.cb = sizeof(STARTUPINFOA);
    StartupInfo.wShowWindow = (WORD)uCmdShow;
    StartupInfo.dwFlags = 0;

    if (!CreateProcessA(NULL,
        (LPSTR)lpCmdLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &StartupInfo,
        &ProcessInformation))
    {
        dosErr = GetLastError();
        return dosErr < 32 ? dosErr : ERROR_BAD_FORMAT;
    }

    if (NULL != UserWaitForInputIdleRoutine)
    {

        UserWaitForInputIdleRoutine(ProcessInformation.hProcess, 10000);
    }

    CloseHandle(ProcessInformation.hProcess);
    CloseHandle(ProcessInformation.hThread);

    return 33;
}

int main()
{

    RegisterWaitForInputIdle(MyWaitForInputIdleRoutine);


    UINT result = MyWinExec("C:\\Windows\\System32\\calc.exe", SW_SHOWNORMAL);

    std::cout << "WinExec returned: " << result << std::endl;

    return 0;
}