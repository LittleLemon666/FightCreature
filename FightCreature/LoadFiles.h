#pragma once
#include <windows.h>    // LPWSTR ConvertString  cvtLPW2stdstring  read_directory
class LoadFiles
{
public:
    LoadFiles();
    ~LoadFiles();
    LPWSTR ConvertString(const std::string& instr);
    bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage);
    void read_directory(const std::string& path, std::vector<std::string>& filePathes, std::string target); //write map in this function
};