#pragma once
#include <windows.h>    // LPWSTR ConvertString  cvtLPW2stdstring  read_directory
class LoadFiles
{
public:
    LoadFiles();
    ~LoadFiles();
    const LPWSTR ConvertString(const std::string& instr);
    const bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage);
    const void read_directory(const std::string& path, std::vector<std::string>& filePathes, std::string target); //write map in this function
};