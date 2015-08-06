#include "JSON.h"

using namespace JSON;


#define F_EXPAND_STORAGE( type ) \
if (_size < sizeof(type)) \
{ \
	if (_var != nullptr) \
	{ \
		free(_var); \
	} \
	_size = sizeof(type); \
	_var = (type*)malloc(_size); \
}


Var::Var(Var && src)
{
	_var = src._var;
	_size = src._size;
	_type = src._type;

	src._var = nullptr;
	src._size = 0;
	src._type = VARTYPE_UNDEFINED;
}


Var::~Var()
{
	if (_var != nullptr)
	{
		free(_var);
	}
}


Var & Var::operator= (const Var & v)
{
	return *this;
}

Var & Var::operator= (const Integer & i)
{
	F_EXPAND_STORAGE(Integer);

	_type = VARTYPE_INTEGER;
	*((Integer*)_var) = i;

	return *this;
}

Var & Var::operator= (const Float &f)
{
	F_EXPAND_STORAGE(Float);

	_type = VARTYPE_FLOAT;
	*((Float*)_var) = f;

	return *this;
}

Var & Var::operator= (const String & s)
{
	if (_size < sizeof(String::value_type) * s.length())
	{
		if (_var != nullptr)
		{
			free(_var);
		}
		_size = s.length() * sizeof(String::value_type);
		_var = (String::value_type*)malloc(_size);
	}

	_type = VARTYPE_STRING;
	strcpy((String::value_type*)_var, s.c_str());

	return *this;
}

Var & Var::operator= (const Array & a)
{
	return *this;
}

Var & Var::operator= (const Object & o)
{
	return *this;
}


Var & Var::operator= (Array * a)
{
	//a._refcount++;

	//_type |= VARTYPE_REFERENCE;
	return *this;
}


Var & Var::operator= (Object * o)
{
	return *this;
}
