#include "MemMan.h"
#include "TlHelp32.h"


bool MemMan::FindGame(const wchar_t* game)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) return NULL;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return 0; 
    }

    do {
        if (_wcsicmp(pe32.szExeFile, game) == 0) {
            CloseHandle(hSnapshot);
            this->processId = pe32.th32ProcessID;
            this->processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);
            return true;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return false;
}

DWORD MemMan::GetProcessId()
{
	return this->processId;
}

HANDLE MemMan::GetProcessHandle()
{
	return this->processHandle;
}

bool MemMan::CloseProcessHandle()
{
    if (!this->processHandle) return false;

    CloseHandle(this->processHandle);
    return true;
}
