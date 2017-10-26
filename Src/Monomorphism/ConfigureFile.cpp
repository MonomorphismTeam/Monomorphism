/*================================================================
Filename: ConfigureFile.cpp
Date: 2017.10.26
Created by AirGuanZ
================================================================*/
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>

#include "Include\ConfigureFile.h"

using namespace std;

namespace
{
    std::string _TrimInline(const std::string &s)
    {
        auto b = s.find_first_not_of(" \t");
        auto e = s.find_last_not_of(" \t");
        if(b == string::npos || e == string::npos || b > e)
            return "";
        return s.substr(b, e - b + 1);
    }
}

bool ConfigureFile::Load(const std::string &filename)
{
    items_.clear();
    ifstream fin(filename, ifstream::in);
    string line;
    string sectionName = "__DefaultSection";
    if(!fin)
        goto FAILED;

    while(getline(fin, line) && fin)
    {
        if((line = _TrimInline(line)).empty() || line[0] == '#') //空行或注释
            continue;
        if(line[0] == '[') //新section
        {
            int end = line.rfind(']');
            if(end == string::npos || end == 1)
                goto FAILED;
            sectionName = _TrimInline(line.substr(1, end - 1));
        }
        else //新item
        {
            auto eq = line.find('=');
            if(eq == string::npos || eq == 0 || eq == line.length() - 1)
                goto FAILED;
            string left = _TrimInline(line.substr(0, eq));
            string right = _TrimInline(line.substr(eq + 1));
            if(left.empty() || right.empty())
                goto FAILED;
            items_[sectionName + "::" + left] = right;
        }
    }

    fin.close();
    return true;

FAILED:
    items_.clear();
    return false;
}

const std::string &ConfigureFile::operator()(const std::string &item) const
{
    return (*this)("__DefaultSection", item);
}

const std::string &ConfigureFile::operator()(const std::string &section, const std::string &item) const
{
    auto it = items_.find(section + "::" + item);
    if(it == items_.end())
        return "";
    return it->second;
}
