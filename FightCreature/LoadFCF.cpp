//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <ctime>
//#include <vector>
//#include <queue>
//#include <conio.h>		// _getch() <= to read input without enter "ENTER" key
//#include <windows.h>
//#include "LoadFCF.h"
//using std::ifstream;
//using namespace std;
//
//LPWSTR ConvertString(const std::string& instr)
//{
//    //Author: scruffybear
//    //URL: https://xionghuilin.com/c-convert-between-string-and-cstring-lpwstr/
//
//    // Assumes std::string is encoded in the current Windows ANSI codepage
//    int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);
//
//    if (bufferlen == 0)
//    {
//        // Something went wrong. Perhaps, check GetLastError() and log.
//        return 0;
//    }
//
//    // Allocate new LPWSTR - must deallocate it later
//    LPWSTR widestr = new WCHAR[bufferlen + 1];
//
//    ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);
//
//    // Ensure wide string is null terminated
//    widestr[bufferlen] = 0;
//
//    // Do something with widestr
//    return widestr;
//    //delete[] widestr;
//}
//
//bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage = CP_ACP)
//{
//    //Author: ArkM
//    //URL: https://www.daniweb.com/programming/software-development/threads/155420/lpwstr-to-std-string-help
//
//    bool res = false;
//    char* p = 0;
//    int bsz;
//    bsz = WideCharToMultiByte(codepage,
//        0,
//        pw, -1,
//        0, 0,
//        0, 0);
//    if (bsz > 0) {
//        p = new char[bsz];
//        int rc = WideCharToMultiByte(codepage,
//            0,
//            pw, -1,
//            p, bsz,
//            0, 0);
//        if (rc != 0) {
//            p[bsz - 1] = 0;
//            s = p;
//            res = true;
//        }
//    }
//    delete[] p;
//    return res;
//}
//
//void read_directory(const std::string& path, vector<string>& filePathes) //write map in this function
//{
//    //Author: MARTIN
//    //URL: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html
//
//    std::string pattern(path);
//    pattern.append("\\*.fcf");
//    string s;
//    WIN32_FIND_DATA data;
//    HANDLE hFind = FindFirstFile(ConvertString(pattern.c_str()), &data);
//    do {
//        cvtLPW2stdstring(s, data.cFileName);
//        cout << s << "\n";
//        filePathes.push_back(path + "\\" + s);
//    } while (FindNextFile(hFind, &data) != 0);
//    FindClose(hFind);
//}