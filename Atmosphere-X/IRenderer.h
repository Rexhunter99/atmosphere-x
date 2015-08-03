#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <cstdint>
#include <map>
#include <string>


class IRenderer
{
public:
	virtual void setFullscreen(bool) = 0;

	// Base Resource interface for the Renderer class and all the other interfaces
	class IResource
	{
	public:

		IResource()
		{
			this->class_name = "Unknown";
		}

		IResource(const std::string & class_name)
		{
			this->class_name = class_name;
		}

		const size_t
			getID()
		{ return this->id; }

		const std::string
			getClassName()
		{ return this->class_name; }

	protected:
		size_t		id;
		std::string	class_name;
	};

	// Texture interface for the Renderer class
	class ITexture :
		public IResource
	{
	public:

	protected:
		unsigned	_width : 16;
		unsigned	_height : 16;
		struct {
			unsigned _red : 4;
			unsigned _green : 4;
			unsigned _blue : 4;
			unsigned _alpha : 4;
		} _bitdepth;
		
		ITexture() : IResource("ITexture") {}
		ITexture(const std::string & class_name) : IResource(class_name) {}
		virtual ~ITexture() {}
	};

	// Font interface for the Renderer class
	class IFont :
		public IResource
	{
	public:

	protected:
		IFont() : IResource("IFont") {}
		virtual ~IFont() = 0;
	};

	// Factory interface for the Renderer class
	class IFactory
	{
	protected:
		std::map<std::string, IResource>	_resources;
	};

protected:
	virtual
		~IRenderer() {}

};

#endif //__RENDERER_H__
