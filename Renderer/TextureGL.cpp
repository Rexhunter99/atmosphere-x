#include "TextureGL.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>


TextureGL::~TextureGL()
{
	glDeleteTextures(1, &_glid);
}

void TextureGL::_construct()
{
	glGenTextures(1, &_glid);
}
