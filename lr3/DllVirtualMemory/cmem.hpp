#include <Windows.h>
#include <string>

using namespace std;

class CMem {
private:
    HWND hWnd;
    HANDLE hWin;
    DWORD procId;
public:
    CMem(HWND hWnd);
    int write(DWORD addr, char* val);
    char* read(DWORD addr);
    ~CMem();
};

CMem::CMem(HWND hWnd) {
    if (!hWnd) {
        MessageBox(NULL, L"Error, in cmem constructor, wrong hwnd passed", L"Error", MB_ICONERROR);
    }
    else {
        this->hWnd = hWnd;
        GetWindowThreadProcessId(hWnd, &procId);
    }
}

int CMem::write(DWORD addr, char* val) {
    int res;
    SIZE_T writtenBytes = 0;
    SIZE_T length = strlen(val);
    hWin = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
    res = WriteProcessMemory(hWin, (void*)addr, (void*)val, length, &writtenBytes);
    return res;
}

char* CMem::read(DWORD addr) {
    char* res = NULL;
    int save, ifRead;
    SIZE_T readenBytes;
    hWin = OpenProcess(PROCESS_VM_READ, false, procId);
    char temp;
    int curSize = 1;
    do {
        readenBytes = 0;
        ifRead = ReadProcessMemory(hWin, (void*)addr, &temp, sizeof(char), &readenBytes);
        if (res) {
            char* tempRes = (char*)malloc(sizeof(char) * curSize);
            memcpy(tempRes, res, curSize * sizeof(char));
            free(res);
            curSize += 1;
            res = (char*)malloc(sizeof(char) * curSize);
            memcpy(res, tempRes, (curSize - 1) * sizeof(char));
            *(res + curSize * sizeof(char)) = temp;
            free(tempRes);
        }
    } while (temp != '\0' && ifRead != FALSE && readenBytes > 0);
    return res;
}

CMem::~CMem() {
    CloseHandle(hWin);
}