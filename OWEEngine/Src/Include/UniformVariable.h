/*================================================================
Filename: UniformVariable.h
Date: 2017.9.24
Created by AirGuanZ
================================================================*/
#ifndef __OWE_UNIFORM_VARIABLE_H__
#define __OWE_UNIFORM_VARIABLE_H__

#include <map>
#include <string>
#include <tuple>
#include <utility>

#include "Common.h"
#include "GLHeaders.h"
#include "TypeOpr.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_UniformAux)

//用重载表述OGL不同类型glUniform使之具有统一形式

inline void _SetUniform(GLint loc, GLfloat fv0)
{
    glUniform1f(loc, fv0);
}
inline void _SetUniform(GLint loc, GLint v0)
{
    glUniform1d(loc, v0);
}
inline void _SetUniform(GLint loc, const glm::vec2 &v)
{
    glUniform2f(loc, v.x, v.y);
}
inline void _SetUniform(GLint loc, const glm::vec3 &v)
{
    glUniform3f(loc, v.x, v.y, v.z);
}
inline void _SetUniform(GLint loc, const glm::vec4 &v)
{
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}
inline void _SetUniform(GLint loc, const glm::mat2x2 &m)
{
    glUniformMatrix2fv(loc, 1, GL_FALSE, &m[0][0]);
}
inline void _SetUniform(GLint loc, const glm::mat3x3 &m)
{
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}
inline void _SetUniform(GLint loc, const glm::mat4x4 &m)
{
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

//提供给uniform variable的容器，用于统一apply操作
class _UniformVariableBase
{
public:
    virtual void Apply(void) const = 0;
    virtual ~_UniformVariableBase(void) { };
};

template<typename _VarType>
class _UniformVariableImpl : public _UniformVariableBase
{
public:
    friend class _UniformVariableManager;

    using VarType = _VarType;

    ~_UniformVariableImpl(void)
    {
        //do nothing
    }

    //设置某个分量的值
    void SetVal(std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> v)
    {
        var_ = v;
    }

    //使用该变量值
    void Apply(void) const
    {
        _SetUniform(loc_, var_);
    }

    void SetAndApply(std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> v)
    {
        SetVal(v);
        Apply();
    }

    //取得整个Uniform variable的值
    Utility::RefIfNotNumeric_t<VarType> GetVal(void)
    {
        return var_;
    }

    //取得整个Uniform variable的值
    std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> GetVal(void) const
    {
        return var_;
    }

private:
    _UniformVariableImpl(GLint loc)
        :loc_(loc)
    {

    }

    GLint loc_;
    VarType var_;
};

template<typename _VarType>
class _UniformVariable
{
public:
    friend class _UniformVariableManager;
    using VarType = _VarType;

    ~_UniformVariable(void)
    {
        //do nothing
    }

    //设置整个Uniform variable的值
    void SetVal(std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> var)
    {
        impl_.SetVal(var);
    }

    //使用该变量值
    void Apply(void) const
    {
        impl_.Apply();
    }

    void SetAndApply(std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> var)
    {
        impl_.SetAndApply(var);
    }

    //取得整个Uniform variable的值
    Utility::RefIfNotNumeric_t<VarType> GetVal(void)
    {
        return impl_.GetVal();
    }

    //取得整个Uniform variable的值
    std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> GetVal(void) const
    {
        return impl_.GetVal();
    }

private:
    _UniformVariable(_UniformVariableImpl<VarType> &impl)
        :impl_(impl)
    {

    }

    _UniformVariableImpl<VarType> &impl_;
};

template<typename VarType>
class _ImmediateUniformVariable
{
public:
    friend class _UniformVariableManager;

    void SetAndApply(std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> var) const
    {
        _SetUniform(loc_, var);
    }

private:
    _ImmediateUniformVariable(GLuint loc)
        :loc_(loc)
    {

    }

    GLuint loc_;
};

__OWE_END_NAMESPACE__(_UniformAux)

template<typename VarType>
using UniformVariable = _UniformAux::_UniformVariable<VarType>;

template<typename VarType>
using ImmediateUniformVariable = _UniformAux::_ImmediateUniformVariable<VarType>;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_UNIFORM_VARIABLE_H__
