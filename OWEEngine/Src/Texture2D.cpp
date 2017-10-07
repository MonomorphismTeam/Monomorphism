/*================================================================
Filename: Texture.cpp
Date: 2017.10.5
By AirGuanZ
================================================================*/
#include <cassert>

#include "Include\Texture2D.h"

__OWE_BEGIN_NAMESPACE__(OWE)
__OWE_BEGIN_NAMESPACE__(_Texture2DAux)

_Texture2D::_Texture2D(void)
	:tex_(0)
{

}

_Texture2D::~_Texture2D(void)
{
	Destroy();
}

void _Texture2D::Initialize(const Desc &desc, void *data)
{
	assert(desc.width > 0 && desc.height > 0);
	Destroy();

	GLint curTex;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &curTex);

	glGenTextures(1, &tex_);
	glBindTexture(GL_TEXTURE_2D, tex_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrapT);
    glTexImage2D(
        GL_TEXTURE_2D, 0, desc.internalFormat,
        static_cast<GLsizei>(desc.width),
        static_cast<GLsizei>(desc.height),
        0, desc.inputFormat, desc.inputType, data);
    width_ = desc.width;
    height_ = desc.height;

	glBindTexture(GL_TEXTURE_2D, curTex);
}

bool _Texture2D::IsAvailable(void) const
{
	return tex_ != 0;
}

void _Texture2D::Destroy(void)
{
	if (IsAvailable())
	{
		glDeleteTextures(1, &tex_);
		tex_ = 0;
	}
}

void _Texture2D::Bind(int slot) const
{
	assert(IsAvailable());
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex_);
}

bool _LoadTexture2DFromFile(const std::string &filename, const _Texture2D::Desc &desc, _Texture2D &tex)
{
    //TODO
    return false;
}

__OWE_END_NAMESPACE__(_Texture2DAux)
__OWE_END_NAMESPACE__(OWE)