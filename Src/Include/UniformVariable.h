/*================================================================
Filename: UniformVariable.h
Date: 2017.9.24
By AirGuanZ
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
inline void _SetUniform(GLint loc, GLfloat fv0, GLfloat fv1)
{
    glUniform2f(loc, fv0, fv1);
}
inline void _SetUniform(GLint loc, GLfloat fv0, GLfloat fv1, GLfloat fv2)
{
    glUniform3f(loc, fv0, fv1, fv2);
}
inline void _SetUniform(GLint loc, GLfloat fv0, GLfloat fv1, GLfloat fv2, GLfloat fv3)
{
    glUniform4f(loc, fv0, fv1, fv2, fv3);
}
inline void _SetUniform(GLint loc, GLint v0)
{
    glUniform1d(loc, v0);
}
inline void _SetUniform(GLint loc, GLint v0, GLint v1)
{
    glUniform2d(loc, v0, v1);
}
inline void _SetUniform(GLint loc, GLint v0, GLint v1, GLint v2)
{
    glUniform3d(loc, v0, v1, v2);
}
inline void _SetUniform(GLint loc, GLint v0, GLint v1, GLint v2, GLint v3)
{
    glUniform4d(loc, v0, v1, v2, v3);
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

template<typename...VarTypes>
class _UniformVariable
{
public:
    template<int I>
    using _VarType = typename std::tuple_element<I, std::tuple<VarTypes...>>::type;

    _UniformVariable(GLint loc)
        :loc_(loc)
    {

    }

    //设置整个Uniform variable的值
    void SetVals(std::add_const_t<Utility::RefIfNotNumeric_t<VarTypes>>... var)
    {
        var_ = std::tuple<VarTypes...>(var...);
    }

    //设置某个分量的值
    template<int I>
    void SetVal(std::add_const_t<Utility::RefIfNotNumeric_t<_VarType<I>>> v)
    {
        std::get<I>(var_) = v;
    }

    //使用该变量值
    void Bind(void)
    {
        _BindAux(std::make_index_sequence<std::tuple_size<decltype(var_)>::value>());
    }

    //取得整个Uniform variable的值
    std::tuple<VarTypes...> &GetVals(void)
    {
        return var_;
    }

    //取得整个Uniform variable的值
    const std::tuple<VarTypes...> &GetVals(void) const
    {
        return var_;
    }

    //取得某个分量的值
    template<int I>
    _VarType<I> &GetVal(void)
    {
        return std::get<I>(var_);
    }

    //取得某个分量的值
    template<int I>
    std::add_const_t<Utility::RefIfNotNumeric_t<_VarType<I>>> GetVal(void) const
    {
        return std::get<I>(var_);
    }

private:
    template<std::size_t...I>
    void _BindAux(std::index_sequence<I...>)
    {
        _SetUniform(loc_, std::get<I>(var_)...);
    }

    GLint loc_;
    std::tuple<VarTypes...> var_;
};

__OWE_END_NAMESPACE__(_UniformAux)

template<typename...VarTypes>
using UniformVariable = _UniformAux::_UniformVariable<VarTypes...>;

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_UNIFORM_VARIABLE_H__