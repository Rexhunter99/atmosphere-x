#pragma once
#ifndef __WINDOW_CLASS_H__
#define __WINDOW_CLASS_H__


/*** Headers: Standard library includes ***/
#include <cstdint>
#include <string>

// Custom "internal project" headers get listed here


/*** Type definitions: Required type definitions go here ***/
typedef void (*funcptr_t)(void);


// Window class
class Window
{
public:

	enum EnumStyles {
		WF_DEFAULT				= 0,
		WF_STYLE_DEFAULT		= 0,
		WF_STYLE_BORDERLESS,
		WF_STYLE_MINIMAL,
		WF_STYLE_SLIM,
		WF_STYLE_BULKY,
		WF_STYLE_FULLSCREEN
	};

	enum EnumPositions {
		WF_POS_DEFAULT			= 0,
		WF_POS_CENTER
	};

	enum EnumEvent {
		WE_CREATE				= 0,
		WE_CLOSE,
		WE_SHOW_OR_HIDE,
		WE_KEY_INPUT,
		WE_MOUSE_INPUT,
		WE_RESIZED,
		WE_MOVED,
		WE_MINIMIZED,
		WE_MAXIMIZED,
		WE_MAX_VALUE
	};

	Window(void);
	Window(Window && src);
	Window(const std::tstring & title, int style, int position, uint16_t width, uint16_t height);
	~Window(void);

	Window & operator = (Window &);

	void addEventListener(unsigned event, funcptr_t callback);
	void setTitle(const std::tstring & title);
	void setSize(const uint16_t & width, const uint16_t & height);
	void setPosition(unsigned flag);
	void setPosition(int32_t x, int32_t y);
	void setProperty(unsigned property, bool value);
	const void *	getNativeHandle() const;
	const void *	getGraphicContext() const;
	bool processEvents(void);
	void processEventsBlocking(void);

protected:
	bool				_valid;
	void*				_native_handle;
	void*				_native_gc;
	funcptr_t			_event[WE_MAX_VALUE];

	#ifdef _WIN32
	friend void WindowEventResponse( uint32_t message, uint16_t param16, uint32_t param32, const Window * window );
	#elif _X11
	friend void WindowEventResponse( Event event, const Window * window );
	#endif

private:
	// Disable the copy constructor, we rely on the C++11 move constructor
	Window(const Window & src);
};

#endif //__WINDOW_CLASS_H__
