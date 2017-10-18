/*================================================================
Filename: UniformVariableManager.h
Date: 2017.9.28
Created by AirGuanZ
================================================================*/
#ifndef __UNIFORM_VARIABLE_MANAGER_H__
#define __UNIFORM_VARIABLE_MANAGER_H__

#include <map>
#include <string>
#include <vector>

#include "Common.h"
#include "GLHeaders.h"
#include "UniformVariable.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_UniformAux)

//IMPROVE: 用StaticList(type, VarTypes)改写_UniformTypeChecker
//这样一长串特化看起来太蠢了

template<typename...VarTypes> struct _UniformTypeCheckerAux
{
    static bool _Check(GLenum type) { return false; }
};

template<> struct _UniformTypeCheckerAux<GLfloat>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT; }
};

template<> struct _UniformTypeCheckerAux<GLint>
{
    static bool _Check(GLenum type) { return type == GL_INT; }
};

template<> struct _UniformTypeCheckerAux<glm::vec2>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT_VEC2; }
};

template<> struct _UniformTypeCheckerAux<glm::vec3>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT_VEC3; }
};

template<> struct _UniformTypeCheckerAux<glm::vec4>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT_VEC4; }
};

template<> struct _UniformTypeCheckerAux<glm::mat2x2>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT_MAT2; }
};

template<> struct _UniformTypeCheckerAux<glm::mat3x3>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT_MAT3; }
};

template<> struct _UniformTypeCheckerAux<glm::mat4x4>
{
    static bool _Check(GLenum type) { return type == GL_FLOAT_MAT4; }
};

template<> struct _UniformTypeCheckerAux<Texture2DBase>
{
    static bool _Check(GLenum type) { return type == GL_SAMPLER_2D; }
};

template<typename VarType>
bool _UniformTypeChecker(GLenum type)
{
    return _UniformTypeCheckerAux<VarType>::_Check(type);
}

class _UniformVariableManager
{
public:
    struct VarInfo
    {
        GLint location;
        GLint tex2DSlot;
        GLenum type;

        _UniformAux::_UniformVariableBase *_var;
    };

    struct UniformNameLengthError { std::string name; };
    struct UniformTypeError { std::string name; GLenum actType; };
    struct UniformNotFoundError { std::string name; };

    explicit _UniformVariableManager(GLuint prog)
        : sampler2DCnt_(0)
    {
        assert(glIsProgram(prog));
        //取得active uniform variable数量
        GLint activeCnt = 0;
        glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &activeCnt);

        //查询每个variable信息
        GLint maxLen;
        glGetProgramiv(prog, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
        std::vector<GLchar> nameBuf(maxLen + 1);
        for(GLint i = 0; i != activeCnt; ++i)
        {
            GLenum type;
            GLsizei nameLen, size;
            glGetActiveUniform(prog, i, maxLen, &nameLen, &size, &type, nameBuf.data());
            //glGetUniformLocation失败说明是个数组或者结构，不被uniform variable支持
            GLint location = glGetUniformLocation(prog, nameBuf.data());
            if(location == -1)
                continue;

            GLint tex2DSlot = 0;
            if(type == GL_SAMPLER_2D)
                tex2DSlot = sampler2DCnt_++;

            Add(nameBuf.data(), { location, tex2DSlot, type, nullptr });
        }
    }

    ~_UniformVariableManager(void)
    {
        for(auto it : vars_)
        {
            if(it.second._var)
                delete it.second._var;
        }
    }

    //添加一个新的uniform variable记录
    void Add(const std::string &name, const VarInfo& info)
    {
        vars_[name] = info;
    }

    //查找一个uniform variable是否存在
    bool FindUniform(const std::string &name) const
    {
        return vars_.find(name) != vars_.end();
    }

    //取得特定uniform variable的box class用于设置它
    template<typename VarType>
    _UniformVariable<VarType> GetUniform(const std::string &name)
    {
        auto it = vars_.find(name);
        if(it == vars_.end())
            throw UniformNotFoundError{ name };
        VarInfo &info = it->second;
        if(!_UniformTypeChecker<VarType>(info.type))
            throw UniformTypeError{ name, info.type };
        if(!info._var)
            info._var = new _UniformVariableImpl<VarType>(info.location, info.tex2DSlot);
        return _UniformVariable<VarType>(*dynamic_cast<_UniformVariableImpl<VarType>*>(info._var));
    }

    void Apply(void) const
    {
        for(auto it : vars_)
        {
            if(it.second._var)
                it.second._var->Apply();
        }
    }

    const std::map<std::string, VarInfo> &_GetAllUniforms(void) const
    {
        return vars_;
    }

private:
    std::map<std::string, VarInfo> vars_;
    int sampler2DCnt_;
};

__OWE_END_NAMESPACE__(_UniformAux)

using UniformVariableManager = _UniformAux::_UniformVariableManager;

__OWE_END_NAMESPACE__(OWE)

#endif //__UNIFORM_VARIABLE_MANAGER_H__
