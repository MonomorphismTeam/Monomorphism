/*================================================================
Filename: FatalError.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __OWE_FATAL_ERROR_H__
#define __OWE_FATAL_ERROR_H__

#include <exception>
#include <string>

#include "Common.h"

__OWE_BEGIN_NAMESPACE__(OWE)

class FatalError
{
public:
    FatalError(const std::string &err)
        :err_(err)
    {

    }

    const char *What(void) const
    {
        return err_.c_str();
    }

private:
    std::string err_;
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_FATAL_ERROR_H__
