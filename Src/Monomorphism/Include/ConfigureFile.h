/*================================================================
Filename: ConfigureFile.h
Date: 2017.10.26
Created by AirGuanZ
================================================================*/
#ifndef __CONFIGURE_FILE_H__
#define __CONFIGURE_FILE_H__

#include <map>
#include <string>

class ConfigureFile
{
public:
    bool Load(const std::string &filename);

    const std::string &operator()(const std::string &item) const;
    const std::string &operator()(const std::string &section, const std::string &item) const;

private:
    std::map<std::string, std::string> items_;
};

#endif //__CONFIGURE_FILE_H__
