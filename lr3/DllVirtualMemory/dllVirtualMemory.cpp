#include "dllVirtualMemory.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}

void replace(char* strToSearch, char* strForReplace) {
    MessageBox(NULL, TEXT("Hello"), TEXT("Test"), MB_OK);
}