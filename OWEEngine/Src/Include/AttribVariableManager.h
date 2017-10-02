/*================================================================
Filename: AttribVariableManager.h
Date: 2017.10.2
By AirGuanZ
================================================================*/
#ifndef __OWE_ATTRIB_VARIABLE_MANAGER_H__
#define __OWE_ATTRIB_VARIABLE_MANAGER_H__

#include <map>
#include <string>

#include "Common.h"
#include "GLHeaders.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_AttribAux)

class _AttribVariableManager
{
public:

private:
    struct _AttribInfo
    {
        GLint location;
        GLenum type;
    };

    GLuint vao_;
    std::map<std::string, _AttribInfo> attribs_;
};

__OWE_END_NAMESPACE__(_AttribAux)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_ATTRIB_VARIABLE_MANAGER_H__