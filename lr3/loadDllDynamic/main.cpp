#include <Windows.h>
#include <iostream>

using namespace std;

void(*replace) (char*, char*);

int main() {

    HINSTANCE hDllInst = LoadLibrary((LPCWSTR)TEXT("../Debug/DllVirtualMemory.dll"));
    if (hDllInst) {
        replace = (void(*)(char*, char*))GetProcAddress(hDllInst, "replace");
        replace("str1", "str2");
    }
    getchar();
    return 0;
}