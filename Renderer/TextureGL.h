#pragma once

#ifndef __TEXTURE_OPENGL_H__
#define __TEXTURE_OPENGL_H__

#include "../Atmosphere-X/IRenderer.h"


class TextureGL :
	public IRenderer::ITexture
{
public:
	~TextureGL();

protected:
	friend class RendererGL2;

	// Initialise an instance of TextureGL with default configuration
	TextureGL() :
		IRenderer::ITexture("TextureGL")
	{
		_construct();
	}

	// Helper function to be called inside the constructor for initialising data
	void _construct();

	uint32_t		_glid;
};

#endif // __TEXTURE_OPENGL_H__