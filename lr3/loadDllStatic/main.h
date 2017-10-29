#pragma once
#include  <iostream>

#define DLL_IMPORT __declspec(dllimport)

extern "C" {
    DLL_IMPORT void replace(char* strToSearch, char* strForReplace);
}