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

    void Clear(void);

    static std::string DefaultSectionName(void)
    {
        return "__DefaultSection";
    }

    template<typename Func>
    void ForEach(Func func, const std::string &section = DefaultSectionName())
    {
        std::string head = section + "::";
        for(const auto &it : items_)
        {
            if(it.first.find(head) != std::string::npos)
                func(it.first.substr(head.length()), it.second);
        }
    }

private:
    std::map<std::string, std::string> items_;
};

#endif //__CONFIGURE_FILE_H__
