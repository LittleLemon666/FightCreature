#include <fstream>
#include <string>
#include <vector>
#include <windows.h>    // LPWSTR ConvertString  cvtLPW2stdstring  read_directory
#include "LoadFiles.h"
using namespace std;

LoadFiles::LoadFiles()
{

}

LoadFiles::~LoadFiles()
{

}

const LPWSTR LoadFiles::ConvertString(const std::string& instr)
{
    //Author: scruffybear
    //URL: https://xionghuilin.com/c-convert-between-string-and-cstring-lpwstr/

    // Assumes std::string is encoded in the current Windows ANSI codepage
    int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);

    if (bufferlen == 0)
    {
        // Something went wrong. Perhaps, check GetLastError() and log.
        return 0;
    }

    // Allocate new LPWSTR - must deallocate it later
    LPWSTR widestr = new WCHAR[bufferlen + 1];

    ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);

    // Ensure wide string is null terminated
    widestr[bufferlen] = 0;

    // Do something with widestr
    return widestr;
    //delete[] widestr;
}

const bool LoadFiles::cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage = CP_ACP)
{
    //Author: ArkM
    //URL: https://www.daniweb.com/programming/software-development/threads/155420/lpwstr-to-std-string-help

    bool res = false;
    char* p = 0;
    int bsz;
    bsz = WideCharToMultiByte(codepage,
        0,
        pw, -1,
        0, 0,
        0, 0);
    if (bsz > 0) {
        p = new char[bsz];
        int rc = WideCharToMultiByte(codepage,
            0,
            pw, -1,
            p, bsz,
            0, 0);
        if (rc != 0) {
            p[bsz - 1] = 0;
            s = p;
            res = true;
        }
    }
    delete[] p;
    return res;
}

const void LoadFiles::read_directory(const std::string& path, std::vector<std::string>& filePathes, std::string target) //write map in this function
{
    //Author: MARTIN
    //URL: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html

    std::string pattern(path);
    pattern.append(target);
    string s;
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(ConvertString(pattern.c_str()), &data);
    do {
        cvtLPW2stdstring(s, data.cFileName);
        if (s.length() == 0 || s[0] == '?')
        {
            break;
        }
        filePathes.push_back(path + "\\" + s);
    } while (FindNextFile(hFind, &data) != 0);
    FindClose(hFind);
}