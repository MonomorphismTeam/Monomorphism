/*================================================================
Filename: AttribVariableManager.h
Date: 2017.10.2
Created by AirGuanZ
================================================================*/
#ifndef __OWE_ATTRIB_VARIABLE_MANAGER_H__
#define __OWE_ATTRIB_VARIABLE_MANAGER_H__

#include <cassert>
#include <map>
#include <string>
#include <vector>

#include <glm\glm.hpp>

#include "Common.h"
#include "GLHeaders.h"
#include "VertexBuffer.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_AttribAux)

//IMPROVE：引入机制避免屡次切换当前活动的VAO
//要是OGL 4.5就好了

//IMPROVE：改进_VertexAttribSize和_VertexAttribType的写法
//这样扩展起来太不方便

template<typename AttribType>
inline GLint _VertexAttribSize(void);

template<> inline GLint _VertexAttribSize<GLfloat>(void)
{
    return 1;
}

template<> inline GLint _VertexAttribSize<glm::vec2>(void)
{
    return 2;
}

template<> inline GLint _VertexAttribSize<glm::vec3>(void)
{
    return 3;
}

template<> inline GLint _VertexAttribSize<glm::vec4>(void)
{
    return 4;
}

template<> inline GLint _VertexAttribSize<GLint>(void)
{
    return 1;
}

template<> inline GLint _VertexAttribSize<glm::ivec2>(void)
{
    return 2;
}

template<> inline GLint _VertexAttribSize<glm::ivec3>(void)
{
    return 3;
}

template<> inline GLint _VertexAttribSize<glm::ivec4>(void)
{
    return 4;
}

template<typename AttribType>
inline GLenum _VertexAttribType(void);

template<> inline GLenum _VertexAttribType<GLfloat>(void)
{
    return GL_FLOAT;
}

template<> inline GLenum _VertexAttribType<glm::vec2>(void)
{
    return GL_FLOAT;
}

template<> inline GLenum _VertexAttribType<glm::vec3>(void)
{
    return GL_FLOAT;
}

template<> inline GLenum _VertexAttribType<glm::vec4>(void)
{
    return GL_FLOAT;
}

template<> inline GLenum _VertexAttribType<GLint>(void)
{
    return GL_INT;
}

template<> inline GLenum _VertexAttribType<glm::ivec2>(void)
{
    return GL_INT;
}

template<> inline GLenum _VertexAttribType<glm::ivec3>(void)
{
    return GL_INT;
}

template<> inline GLenum _VertexAttribType<glm::ivec4>(void)
{
    return GL_INT;
}

template<typename Type>
inline GLenum _VertexAttribTypeChecker(void);

template<> inline GLenum _VertexAttribTypeChecker<GLfloat>(void)
{
    return GL_FLOAT;
}

template<> inline GLenum _VertexAttribTypeChecker<glm::vec2>(void)
{
    return GL_FLOAT_VEC2;
}

template<> inline GLenum _VertexAttribTypeChecker<glm::vec3>(void)
{
    return GL_FLOAT_VEC3;
}

template<> inline GLenum _VertexAttribTypeChecker<glm::vec4>(void)
{
    return GL_FLOAT_VEC4;
}

template<> inline GLenum _VertexAttribTypeChecker<GLint>(void)
{
    return GL_INT;
}

template<> inline GLenum _VertexAttribTypeChecker<glm::ivec2>(void)
{
    return GL_INT_VEC2;
}

template<> inline GLenum _VertexAttribTypeChecker<glm::ivec3>(void)
{
    return GL_INT_VEC3;
}

template<> inline GLenum _VertexAttribTypeChecker<glm::ivec4>(void)
{
    return GL_INT_VEC4;
}

template<typename ClassType, typename MemberType>
GLintptr _MemOffset(const MemberType ClassType::* pMem)
{
    return reinterpret_cast<GLintptr>(&((reinterpret_cast<ClassType*>(0)).*pMem));
}

template<typename _AttribType>
class _AttribVariable
{
public:
    friend class _AttribVariableManager;

    template<bool _Dynamic, bool _Copy>
    void SetBuffer(const VertexBuffer<_AttribType, _Dynamic, _Copy> &vtxBuf)
    {
        GLint curVAO, curBuf;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &curVAO);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &curBuf);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vtxBuf._Unsafe_GetID());

        glEnableVertexAttribArray(loc_);
        glVertexAttribPointer(loc_,
            _VertexAttribSize<_AttribType>(),
            _VertexAttribType<_AttribType>(),
            GL_FALSE, sizeof(_AttribType), nullptr);

        glBindVertexArray(curVAO);
        glBindBuffer(GL_ARRAY_BUFFER, curBuf);
    }

    template<typename VBElem, bool _Dynamic, bool _Copy>
    void SetBuffer(const VertexBuffer<VBElem, _Dynamic, _Copy> &&vtxBuf, _AttribType VBElem::* pMem)
    {
        GLint curVAO;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &curVAO);
        glBindVertexArray(vao_);

        //把attribute location用作vertex buffer的binding point
        glBindVertexBuffer(idx_, vtxBuf._Unsafe_GetID(), _MemOffset(pMem), sizeof(VBElem));
        glVertexAttribFormat(loc_, _VertexAttribSize<_AttribType>(), _VertexAttribType<_AttribType>(), GL_FALSE, sizeof(VBElem));
        glVertexAttribBinding(loc_, idx_);
        glEnableVertexAttribArray(loc_);

        glBindVertexArray(curVAO);
    }

private:
    _AttribVariable(GLuint vao, GLint loc, GLint idx)
        :vao_(vao), loc_(loc), idx_(idx)
    {

    }

    GLuint vao_;
    GLint loc_;
    GLint idx_;
};

class _AttribVariableManager
{
public:
    struct _AttribInfo
    {
        GLint location;
        GLenum type;
        GLint idx;
    };

    struct AttribTypeError { std::string name; GLenum type; };
    struct AttribNotFoundError { std::string name; };

    _AttribVariableManager(GLint prog)
    {
        assert(glIsProgram(prog));

        glGenVertexArrays(1, &vao_);

        GLint activeCnt;
        glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTES, &activeCnt);

        GLint maxLen;
        glGetProgramiv(prog, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen);
        std::vector<GLchar> nameBuf(maxLen + 1);
        for(GLint i = 0; i != activeCnt; ++i)
        {
            GLenum type;
            GLsizei nameLen, size;
            glGetActiveAttrib(prog, i, maxLen + 1, &nameLen, &size, &type, nameBuf.data());
            GLint location = glGetAttribLocation(prog, nameBuf.data());
            if(location == -1)
                continue;

            Add(nameBuf.data(), { location, type, static_cast<GLint>(attribs_.size()) });
        }
    }

    ~_AttribVariableManager(void)
    {
        glDeleteVertexArrays(1, &vao_);
    }

    void Add(const std::string &name, const _AttribInfo &info)
    {
        attribs_[name] = info;
    }

    bool FindAttrib(const std::string &name) const
    {
        return attribs_.find(name) != attribs_.end();
    }

    template<typename AttribType>
    _AttribVariable<AttribType> GetAttrib(const std::string &name) const
    {
        auto it = attribs_.find(name);
        if(it == attribs_.end())
            throw AttribNotFoundError{ name };
        const _AttribInfo &info = it->second;
        if(_VertexAttribTypeChecker<AttribType>() != info.type)
            throw AttribTypeError{ name, info.type };
        return _AttribVariable<AttribType>(vao_, info.location, info.idx);
    }

    void Bind(void) const
    {
        glBindVertexArray(vao_);
    }

    void Unbind(void) const
    {
        glBindVertexArray(0);
    }

    const std::map<std::string, _AttribInfo> &_GetAllAttribs(void) const
    {
        return attribs_;
    }

private:

    GLuint vao_;
    std::map<std::string, _AttribInfo> attribs_;
};

__OWE_END_NAMESPACE__(_AttribAux)

template<typename AttribType>
using AttribVariable = _AttribAux::_AttribVariable<AttribType>;

using AttribVariableManager = _AttribAux::_AttribVariableManager;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_ATTRIB_VARIABLE_MANAGER_H__
