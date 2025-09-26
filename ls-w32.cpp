#include <iostream>
#include <windows.h>
#include <string>

void ListFiles(const std::wstring& directory, const bool& showHidden, const bool& recursive);

int wmain(int argc, wchar_t* argv[]) {
    std::wstring path = L".";
    bool showHidden = false;
    bool recursive = false;

    for (int i = 1; i < argc; i++) {
        std::wstring arg = argv[i];
        if (arg == L"--show-hidden") {
            showHidden = true;
        }
        else if (arg == L"-r") {
            recursive = true;
        } 
        else {
            path = arg;
        }
    }
    ListFiles(path, showHidden, recursive);
}

void ListFiles(const std::wstring& directory, const bool& showHidden, const bool& recursive) {
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

        if (findData.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) {
            if (!showHidden) {
                continue;
            }
        }

        std::wstring fullPath = directory + L"\\" + fileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::wcout << L"[DIR] " << fullPath << std::endl;
            if (recursive) {
                ListFiles(fullPath, showHidden, recursive);
            }
        }
        else {
            std::wcout << L"        " << fullPath << std::endl;
        }
    } while (FindNextFileW(hFind, &findData));

    FindClose(hFind);
}