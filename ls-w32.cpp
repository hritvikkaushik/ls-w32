// ls-w32.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <string>

void ListFiles(const std::wstring& directory);

int wmain(int argc, wchar_t* argv[]) {
    std::wstring path = (argc > 1) ? argv[1] : L".";
    ListFiles(path);
}

void ListFiles(const std::wstring& directory) {
    std::wstring searchPath = directory + L"\\*";
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open directory: " << directory << std::endl;
        return;
    }

    do {
        std::wstring fileName = findData.cFileName;

        if (fileName == L"." || fileName == L"..") {
            continue;
        }

        std::wstring fullPath = directory + L"\\" + fileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::wcout << L"[DIR] " << fullPath << std::endl;
            ListFiles(fullPath);
        }
        else {
            std::wcout << L"        " << fullPath << std::endl;
        }
    } while (FindNextFileW(hFind, &findData));

    FindClose(hFind);
}