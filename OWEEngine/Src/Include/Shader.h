/*================================================================
Filename: Shader.h
Date: 2017.9.21
Created by AirGuanZ
================================================================*/
#ifndef __OWE_SHADER_H__
#define __OWE_SHADER_H__

#include <cassert>
#include <memory>

#include "AttribVariableManager.h"
#include "Common.h"
#include "GLHeaders.h"
#include "ShaderAux.h"
#include "UniformVariableManager.h"

__OWE_BEGIN_NAMESPACE__(OWE)

template<int ShaderStageName>
using ShaderStage = _ShaderAux::_ShaderStage<ShaderStageName>;

class Shader : public _ShaderAux::_ShaderStageName
{
public:
    using Ptr = std::shared_ptr<Shader>;
    using InitState = _ShaderAux::ShaderInitState;

    Shader(void) = default;
    Shader(const Shader&) = delete;
    Shader &operator=(const Shader&) = delete;
    ~Shader(void);

    template<typename...Args>
    InitState Initialize(std::string &err, Args&...args)
    {
        assert(!IsAvailable());
        return _ShaderAux::Initialize(program_, err, args...);
    }

    void Destroy(void);
    bool IsAvailable(void) const;

    void Bind(void) const;
    void Unbind(void) const;
    bool IsBound(void) const;

    UniformVariableManager CreateUniformMgr(void) const;
    AttribVariableManager CreateAttribMgr(void) const;

    GLuint _Unsafe_GetProgramName(void) const;

private:
    GLuint program_ = 0;
};

__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_SHADER_H__
