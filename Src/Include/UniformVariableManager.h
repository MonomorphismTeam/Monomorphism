/*================================================================
Filename: UniformVariableManager.h
Date: 2017.9.28
By AirGuanZ
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

//TODO: 完成_UniformTypeChecker
//可以用tuple形参来进行多个模板参数包的分隔
template<typename...Dsts, typename...Srcs>
bool _UniformTypeCheckerAux(GLenum type, const std::tuple<Dsts...> &, const std::pair<std::tuple<Srcs...>, GLenum> &src)
{
    return false;
}

template<typename...Dsts>
bool _UniformTypeCheckerAux(GLenum type, const std::tuple<Dsts...> &, const std::pair<std::tuple<Dsts...>, GLenum> &src)
{
    return type == src.second;
}

template<typename...Dsts, typename T, typename...Others>
bool _UniformTypeCheckerAux(GLenum type, const std::tuple<Dsts...> &fst, const T &src, std::add_const_t<Others&>...others)
{
    return _UniformTypeCheckerAux(type, fst, src) || _UniformTypeCheckerAux(type, fst, others...);
}

class _UniformVariableManager
{
public:
    struct VarInfo
    {
        GLint location;
        GLenum type;
    };

    struct UniformTypeError { std::string name; GLenum actType; };
    struct UniformNotFoundError { std::string name; };

    _UniformVariableManager(GLuint prog);

    ~_UniformVariableManager(void)
    {
        //do nothing
    }

    void Add(const std::string &name, const VarInfo& info)
    {
        vars_[name] = info;
    }

    bool FindUniform(const std::string &name) const
    {
        return vars_.find(name) != vars_.end();
    }

    template<typename...VarTypes>
    _UniformVariable<VarTypes...> GetUniform(const std::string &name) const
    {
        auto it = vars_.find(name);
        if(it == vars_.end())
            throw UniformNotFoundError{ name };
        if(!_UniformTypeChecker<VarTypes...>(it->second.type))
            throw UniformTypeError{ name, it->second.type };
        return _UniformVariable<VarTypes...>(it->second.location);
    }

private:
    std::map<std::string, VarInfo> vars_;
};

__OWE_END_NAMESPACE__(_UniformAux)

using UniformVariableManager = _UniformAux::_UniformVariableManager;

__OWE_END_NAMESPACE__(OWE)

#endif //__UNIFORM_VARIABLE_MANAGER_H__