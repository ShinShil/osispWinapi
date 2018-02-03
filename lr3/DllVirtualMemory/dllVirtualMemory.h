#pragma once
#include <Windows.h>

#define DLL_EXPORT __declspec(dllexport)

extern "C" {
    DLL_EXPORT void replace(char* strToSearch, char* strForReplace);
    DLL_EXPORT void helloWorld(char* strToSearch, char* strForReplace);
}