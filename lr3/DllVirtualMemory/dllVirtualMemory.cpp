#include "dllVirtualMemory.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}

void helloWorld(char* strToSearch, char* strForReplace) {
    MessageBox(NULL, TEXT("Hello"), TEXT("Test"), MB_OK);
}

void replace(char* strToSearch, char* strForReplace) {
    MEMORY_BASIC_INFORMATION virtualMemoryInfo;
    int memorySize = VirtualQuery(NULL, &virtualMemoryInfo, sizeof(virtualMemoryInfo));
}