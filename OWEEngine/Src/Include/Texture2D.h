/*================================================================
Filename: Texture2D.h
Date: 2017.10.4
By AirGuanZ
================================================================*/
#ifndef __OWE_TEXTURE_2D_H__
#define __OWE_TEXTURE_2D_H__

#include "Common.h"
#include "GLHeaders.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_Texture2DAux)

class _Texture2D
{
public:
	struct Desc
	{
		int width;
		int height;

		GLenum inputFormat;
        GLenum inputType;
		GLint internalFormat = GL_RGBA;

        GLint minFilter = GL_NEAREST;
        GLint magFilter = GL_NEAREST;

        GLint wrapS = GL_CLAMP_TO_EDGE;
        GLint wrapT = GL_CLAMP_TO_EDGE;
	};

	_Texture2D(void);
	_Texture2D(const _Texture2D&) = delete;
	_Texture2D &operator=(const _Texture2D&) = delete;
	~_Texture2D(void);

	void Initialize(const Desc &desc, void *data);
	bool IsAvailable(void) const;
	void Destroy(void);

	void Bind(int slot) const;

private:
    int width_;
    int height_;
	GLuint tex_;
};

__OWE_END_NAMESPACE__(_Texture2DAux)
__OWE_END_NAMESPACE__(OWE)

#endif //__OWE_TEXTURE_2D_H__
