/*================================================================
Filename: AttribVariableManager.h
Date: 2017.10.2
By AirGuanZ
================================================================*/
#ifndef __OWE_ATTRIB_VARIABLE_MANAGER_H__
#define __OWE_ATTRIB_VARIABLE_MANAGER_H__

#include <cassert>
#include <map>
#include <string>

#include "Common.h"
#include "GLHeaders.h"
#include "VertexBuffer.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_AttribAux)

//IMPROVE：引入机制避免屡次切换当前活动的VAO
//要是OGL 4.5就好了

//TODO：_VertexAttribSize和_VertexAttribType

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

        glVertexAttribPointer(loc_,
            _VertexAttribSize<_AttribType>(),
            _VertexAttribType<_AttribType>(),
            GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(loc_);

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
        glBindVertexBuffer(idx_, vtxBuf._Unsafe_GetID(), (GLintptr)(&(((VBElem*)0).*pMem)), sizeof(VBElem));
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
    };

private:

    GLuint vao_;
    std::map<std::string, _AttribInfo> attribs_;
};

__OWE_END_NAMESPACE__(_AttribAux)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_ATTRIB_VARIABLE_MANAGER_H__