/*================================================================
Filename: ASCIIFile.h
Date: 2017.9.14
By AirGuanZ
================================================================*/
#ifndef __OWE_ASCII_FILE_H__
#define __OWE_ASCII_FILE_H__

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(Utility)

class ASCIIFile
{
public:
    static bool Read(const std::string &filename, std::string &rt)
    {
        std::ifstream fin(filename, std::ifstream::in);
        if(!fin)
            return false;
        rt = std::string(std::istreambuf_iterator<char>(fin),
                         std::istreambuf_iterator<char>());
        return true;
    }

    static bool Write(const std::string &filename, const std::string &str)
    {
        std::ofstream fout(filename, std::ofstream::trunc);
        if(!fout)
            return false;
        std::copy(std::begin(str), std::end(str),
                  std::ostreambuf_iterator<char>(fout));
        return true;
    }
};

__OWE_END_NAMESPACE__(Utility)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_ASCII_FILE_H__