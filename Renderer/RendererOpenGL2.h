#pragma once

#ifndef __RENDERER_OPENGL2_H__
#define __RENDERER_OPENGL2_H__

#include "../Atmosphere-X/IRenderer.h"


class RendererGL2 :
	public IRenderer
{
public:
	RendererGL2(void * data);
	~RendererGL2();
	
	void setFullscreen(bool go_fullscreen);

	void free();


private:

	class Factory :
		public IFactory
	{
	private:
		Factory();

	public:
		static IFactory * const get()
		{
			static Factory f;
			return &f;
		}



		~Factory() { this->_resources.clear(); }

	};
};

// Create an instance of the derived IRenderer
extern "C"
{
	__declspec(dllexport) IRenderer * Create(void *);
}

#endif //__RENDERER_OPENGL2_H__
