#pragma once

#ifndef __JSON_H__
#define __JSON_H__

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#define JS_MAX_REF_COUNT		2048
#define JS_MAX_LOOP				2048


namespace JSON
{

	typedef int64_t		Integer;
	typedef double		Float;
	typedef std::string	String;
	class Object;
	class Array;

	typedef enum EVarType
	{
		// Special type that defines that a variable is undefined or has no implementation (unsupported)
		VARTYPE_UNDEFINED		= 0,
		// Null type, used to represent Objects that do not exist but are not undefined
		VARTYPE_NULL,
		// Standard data types
		VARTYPE_INTEGER,
		VARTYPE_FLOAT,
		VARTYPE_STRING,
		// Core implementation of Object data-type
		VARTYPE_OBJECT,
		// Array object data-type
		VARTYPE_ARRAY,
		// Function object data-type
		VARTYPE_FUNCTION,
		// Special type that references a 'complex' data type such as Object or Function
		VARTYPE_REFERENCE		= 0x8000
	} var_t;

	// JavaScript/JSON 'Native' Variable
	class Var
	{
	public:

		Var() : _var(nullptr), _size(0), _type(VARTYPE_UNDEFINED) {}
		Var(Var &&);
		~Var();

		Var & operator= (const Var &);
		Var & operator= (const Integer &);
		Var & operator= (const Float &);
		Var & operator= (const String &);
		Var & operator= (const Array &);
		Var & operator= (const Object &);
		Var & operator= (Array *);
		Var & operator= (Object *);

		void operator+= (const Var &);
		void operator+= (const Integer &);
		void operator+= (const Float &);
		void operator+= (const Var &);

		friend std::ostream& operator<< (std::ostream &os, const Var &v);

	private:
		Var(const Var &);

		void *	_var;
		size_t	_size;
		var_t	_type;
	};

	// JavaScript/JSON 'Native' Object
	class Object
	{
	public:

		Object();
		Object(Object &&);
		~Object();
		
		Var & operator[] (String i);

		friend std::ostream& operator<< (std::ostream &os, const Object &o);

	private:
		Object(const Object &);

		std::map<std::string, Var>	_properties;

		int		_refcount;
	};

	// JavaScript/JSON 'Native' Array
	class Array
	{
	public:

		Array();
		Array(Array &&);
		~Array();

		Var & operator[] (Integer i);

		friend std::ostream& operator<< (std::ostream &os, const Array &a);

	private:
		Array(const Array & a) {}

		std::vector<Var>	_array;

		int		_refcount;
	};

	Var parse_string(const std::string & json);
	Var parse_file(const std::string & json_file);



};

#ifdef __JAVASCRIPT_ENGINE__

namespace JavaScript
{
	class Engine1;
	class Engine2;
	class Engine3;
	class Engine4;
	class Engine5;
	class Engine6_x; // or Engine6_exp 
	// Default Engine alias to the most recent stable build, which is Engine5
	typedef Engine5 Engine;

	bool evaluate(const std::string & script_string);
	bool evaluate(const Var & v);
	
	// Evaluate aliases
	inline bool eval(const std::string & script_string) { return evaluate(script_string); }
	inline bool eval(const Var & v) { return evaluate(v); }
};

#if !defined(JS_NO_SHORTHAND_NAMESPACES)
#define JScript JavaScript
#define JS JavaScript
#endif //JS_NO_SHORTHAND_NAMESPACES

#endif // __JAVASCRIPT_ENGINE__

#endif // __JSON_H__