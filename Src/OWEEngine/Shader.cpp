/*================================================================
Filename: Shader.cpp
Date: 2017.9.21
Created by AirGuanZ
================================================================*/
#include "Include\Shader.h"

__OWE_BEGIN_NAMESPACE__(OWE)

const int _ShaderAux::_ShaderStageName::VS;
const int _ShaderAux::_ShaderStageName::GS;
const int _ShaderAux::_ShaderStageName::FS;

Shader::~Shader(void)
{
    if(IsAvailable())
        Destroy();
}

bool Shader::IsAvailable(void) const
{
    return program_ != 0;
}

void Shader::Destroy(void)
{
    assert(IsAvailable());
    glDeleteProgram(program_);
    program_ = 0;
}

void Shader::Bind(void) const
{
    assert(IsAvailable());
    glUseProgram(program_);
}

void Shader::Unbind(void) const
{
    glUseProgram(0);
}

bool Shader::IsBound(void) const
{
    if(!IsAvailable())
        return false;
    GLint cur;
    glGetIntegerv(GL_CURRENT_PROGRAM, &cur);
    return cur == program_;
}

Shader::UniformMgrPtr Shader::GetUniformMgrPtr(void) const
{
    return std::make_shared<UniformVariableManager>(program_);
}

Shader::AttribMgrPtr Shader::GetAttribMgrPtr(void) const
{
    return std::make_shared<AttribVariableManager>(program_);
}

GLuint Shader::_Unsafe_GetProgramName(void) const
{
    return program_;
}

__OWE_END_NAMESPACE__(OWE)
