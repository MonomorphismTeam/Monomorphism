/*================================================================
Filename: ShaderAux.h
Date: 2017.9.21
By AirGuanZ
================================================================*/
#ifndef __OWE_SHADER_AUX_H__
#define __OWE_SHADER_AUX_H__

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include "Common.h"
#include "GLHeaders.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_ShaderAux)

class _ShaderStageName
{
public:
    static const int VS = 0;
    static const int GS = 1;
    static const int FS = 2;
};

enum class StageInitState
{
    Success,
    ErrorInCreating,
    ErrorInCompiling
};

template<int Stage>
class _ShaderStage;

class _ShaderStageBase
{
public:
    using InitState = ::OWE::_ShaderAux::StageInitState;

    _ShaderStageBase(void) = default;
    ~_ShaderStageBase(void)
    {
        if(IsAvailable())
            DestroyShader();
    }

    bool IsAvailable(void) const
    {
        return shader_ != 0;
    }

    void DestroyShader(void)
    {
        assert(IsAvailable());
        glDeleteShader(shader_);
        shader_ = 0;
    }

    void AttachProgram(GLuint program) const
    {
        assert(IsAvailable());
        glAttachShader(program, shader_);
    }

    void DetachProgram(GLuint program) const
    {
        assert(IsAvailable());
        glDetachShader(program, shader_);
    }

protected:
    InitState _InitShaderStage(GLenum shaderType, const std::string &src, std::string &err)
    {
        assert(!IsAvailable());

        err = "";
        GLint result = 0;
        GLuint shader = glCreateShader(shaderType);
        if(!shader)
            return InitState::ErrorInCreating;

        const char *_src = src.c_str();
        glShaderSource(shader, 1, &_src, nullptr);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if(result != GL_TRUE)
        {
            GLint logLen;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
            std::vector<char> log(logLen + 1);
            glGetShaderInfoLog(shader, log.size(), nullptr, log.data());
            err = log.data();
            glDeleteShader(shader);
            return InitState::ErrorInCompiling;
        }

        shader_ = shader;
        return InitState::Success;
    }

private:
    GLuint shader_ = 0;
};

template<>
class _ShaderStage<_ShaderStageName::VS> : public _ShaderStageBase
{
public:
    _ShaderStage(void) : _ShaderStageBase()
    {

    }

    InitState Init(const std::string &src, std::string &err)
    {
        return _InitShaderStage(GL_VERTEX_SHADER, src, err);
    }
};

template<>
class _ShaderStage<_ShaderStageName::GS> : public _ShaderStageBase
{
public:
    _ShaderStage(void) : _ShaderStageBase()
    {

    }

    InitState Init(const std::string &src, std::string &err)
    {
        return _InitShaderStage(GL_GEOMETRY_SHADER, src, err);
    }
};

template<>
class _ShaderStage<_ShaderStageName::FS> : public _ShaderStageBase
{
public:
    _ShaderStage(void) : _ShaderStageBase()
    {

    }

    InitState Init(const std::string &src, std::string &err)
    {
        return _InitShaderStage(GL_FRAGMENT_SHADER, src, err);
    }
};

__OWE_END_NAMESPACE__(_ShaderAux)

//type-rich string for shader source
struct VSSrc
{
    enum { Stage = _ShaderAux::_ShaderStageName::VS };
    VSSrc(const std::string &s) :src(s) { }
    const std::string &src;
};

struct GSSrc
{
    enum { Stage = _ShaderAux::_ShaderStageName::GS };
    GSSrc(const std::string &s) :src(s) { }
    const std::string &src;
};

struct FSSrc
{
    enum { Stage = _ShaderAux::_ShaderStageName::FS };
    FSSrc(const std::string &s) :src(s) { }
    const std::string &src;
};

__OWE_BEGIN_NAMESPACE__(_ShaderAux)

enum class ShaderInitState
{
    Success,
    ErrorInCreatingShader,
    ErrorInCompilingShader,
    ErrorInCreatingProgram,
    ErrorInLinkingPorgram
};

inline ShaderInitState CVTState(StageInitState state)
{
    const static ShaderInitState rt[] =
    {
        ShaderInitState::Success,
        ShaderInitState::ErrorInCreatingShader,
        ShaderInitState::ErrorInCompilingShader
    };
    return rt[static_cast<int>(state)];
}

template<int S>
ShaderInitState _AttachShader(GLuint &prog, std::string &err, const _ShaderStage<S> &stage)
{
    assert(glIsProgram(prog) == GL_TRUE && stage.IsAvailable());

    GLint rt;
    stage.AttachProgram(prog);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &rt);
    if(rt != GL_TRUE)
    {
        GLint logLen;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen + 1);
        glGetProgramInfoLog(prog, log.size(), nullptr, log.data());
        err = log.data();
        stage.DetachProgram(prog);
        return ShaderInitState::ErrorInLinkingPorgram;
    }

    stage.DetachProgram(prog);
    return ShaderInitState::Success;
}

template<typename Src, typename...Others>
ShaderInitState _AttachShader(GLuint &prog, std::string &err, const Src &s, const Others&...others)
{
    assert(glIsProgram(prog) == GL_TRUE);

    _ShaderStage<static_cast<int>(Src::Stage)> stage;
    ShaderInitState state = CVTState(stage.Init(s.src, err));
    if(state != ShaderInitState::Success)
        return state;

    return _AttachShader(prog, err, stage, others...);
}

template<int S, typename...Others>
ShaderInitState _AttachShader(GLuint &prog, std::string &err, const _ShaderStage<S> &stage, const Others&...others)
{
    assert(glIsProgram(prog) == GL_TRUE && stage.IsAvailable());

    stage.AttachProgram(prog);
    ShaderInitState state = _AttachShader(prog, err, others...);
    stage.DetachProgram(prog);

    return state;
}

template<typename...Others>
ShaderInitState Initialize(GLuint &prog, std::string &err, const Others&...others)
{
    assert(glIsProgram(prog) == GL_FALSE);
    prog = glCreateProgram();
    err = "";
    ShaderInitState state = _AttachShader(prog, err, others...);
    if(state != ShaderInitState::Success)
        glDeleteProgram(prog);
    return state;
}

__OWE_END_NAMESPACE__(_ShaderAux)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_SHADER_AUX_H__