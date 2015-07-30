#pragma once

#include <cstdint>
#include <exception>
#include <stdexcept>


class Window;


class ExceptionDialog
{
public:
	ExceptionDialog(void);
	ExceptionDialog(const std::exception & e);
	~ExceptionDialog(void);

	int wait();

protected:
	bool			_valid;
	void *			_native_handle;
};
