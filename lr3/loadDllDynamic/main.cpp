#include <Windows.h>
#include <iostream>

using namespace std;

void(*replace) (char*, char*);

void writeStr(char* str);
void replaceStr(char* dest, char* src);

int main() {

    /*HINSTANCE hDllInst = LoadLibrary((LPCWSTR)TEXT("../Debug/DllVirtualMemory.dll"));
    if (hDllInst) {
        replace = (void(*)(char*, char*))GetProcAddress(hDllInst, "replace");
        replace("str1", "str2");
    }*/
    void* address = VirtualAlloc(NULL, 32, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    MEMORY_BASIC_INFORMATION virtualMemoryInfo;
    int memorySize = VirtualQuery(NULL, &virtualMemoryInfo, sizeof(virtualMemoryInfo));

    getchar();
    return 0;
}

void writeStr(char* str) {
    
}

void replaceStr(char* dest, char* src) {

}