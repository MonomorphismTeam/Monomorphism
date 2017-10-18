/*================================================================
Filename: UniformVariable.h
Date: 2017.9.24
Created by AirGuanZ
================================================================*/
#ifndef __OWE_UNIFORM_VARIABLE_H__
#define __OWE_UNIFORM_VARIABLE_H__

#include <cassert>
#include <map>
#include <string>
#include <tuple>
#include <utility>

#include "Common.h"
#include "GLHeaders.h"
#include "Texture2DBase.h"
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
class _UniformVariableTexture2DBinder
{
public:
    _UniformVariableTexture2DBinder(GLint texSlot)
    {
        //do nothing
    }

    void SetUniformValue(GLint loc, const _VarType &v) const
    {
        _SetUniform(loc, v);
    }
};

template<>
class _UniformVariableTexture2DBinder<Texture2DBase>
{
public:
    _UniformVariableTexture2DBinder(GLint texSlot)
        : uniformTexSlot_(texSlot)
    {

    }

    void SetUniformValue(GLint loc, Texture2DBase tex) const
    {
        assert(tex.IsAvailable());
        tex.Bind(uniformTexSlot_);
        _SetUniform(loc, uniformTexSlot_);
    }

private:
    GLint uniformTexSlot_;
};

template<typename _VarType>
class _UniformVariableImpl : public _UniformVariableBase, public _UniformVariableTexture2DBinder<_VarType>
{
public:
    friend class _UniformVariableManager;

    using VarType = _VarType;

    ~_UniformVariableImpl(void)
    {
        //do nothing
    }

    //设置值
    void SetVal(const VarType &v)
    {
        var_ = v;
    }

    //使用该变量值
    void Apply(void) const
    {
        SetUniformValue(loc_, var_);
    }

    void SetAndApply(const VarType &v)
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
    _UniformVariableImpl(GLint loc, GLint texSlot)
        : loc_(loc), _UniformVariableTexture2DBinder(texSlot)
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

    explicit _UniformVariable(void)
        : impl_(nullptr)
    {

    }

    ~_UniformVariable(void)
    {
        //do nothing
    }

    //设置整个Uniform variable的值
    void SetVal(const VarType &var)
    {
        impl_->SetVal(var);
    }

    //使用该变量值
    void Apply(void) const
    {
        impl_->Apply();
    }

    void SetAndApply(const VarType &var)
    {
        impl_->SetAndApply(var);
    }

    //取得整个Uniform variable的值
    Utility::RefIfNotNumeric_t<VarType> GetVal(void)
    {
        return impl_->GetVal();
    }

    //取得整个Uniform variable的值
    std::add_const_t<Utility::RefIfNotNumeric_t<VarType>> GetVal(void) const
    {
        return impl_->GetVal();
    }

private:
    explicit _UniformVariable(_UniformVariableImpl<VarType> &impl)
        : impl_(&impl)
    {

    }

    _UniformVariableImpl<VarType> *impl_;
};

__OWE_END_NAMESPACE__(_UniformAux)

template<typename VarType>
using UniformVariable = _UniformAux::_UniformVariable<VarType>;

using UniformTexture2D = Texture2DBase;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_UNIFORM_VARIABLE_H__
