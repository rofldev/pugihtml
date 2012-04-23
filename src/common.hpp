/**
 * pugihtml parser - version 1.0
 * --------------------------------------------------------
 * Copyright (c) 2012 Adgooroo, LLC (kgantchev [AT] adgooroo [DOT] com)
 *
 * This library is distributed under the MIT License. See notice in license.txt
 *
 * This work is based on the pugxml parser, which is: 
 * Copyright (C) 2006-2010, by Arseny Kapoulkine (arseny [DOT] kapoulkine [AT] gmail [DOT] com)
 */

#ifndef COMMON_HPP
#define COMMON_HPP
#include "pugiconfig.hpp"

#ifndef PUGIHTML_NO_STL
namespace std
{
	struct bidirectional_iterator_tag;

#ifdef __SUNPRO_CC
	// Sun C++ compiler has a bug which forces template argument names in forward declarations to be the same as in actual definitions
	template <class _T> class allocator;
	template <class _charT> struct char_traits;
	template <class _charT, class _Traits> class basic_istream;
	template <class _charT, class _Traits> class basic_ostream;
	template <class _charT, class _Traits, class _Allocator> class basic_string;
#else
	// Borland C++ compiler has a bug which forces template argument names in forward declarations to be the same as in actual definitions
	template <class _Ty> class allocator;
	template <class _Ty> struct char_traits;
	template <class _Elem, class _Traits> class basic_istream;
	template <class _Elem, class _Traits> class basic_ostream;
	template <class _Elem, class _Traits, class _Ax> class basic_string;
#endif

	// Digital Mars compiler has a bug which requires a forward declaration for explicit instantiation (otherwise type selection is messed up later, producing link errors)
	// Also note that we have to declare char_traits as a class here, since it's defined that way
#ifdef __DMC__
	template <> class char_traits<char>;
#endif
}
#endif

// Macro for deprecated features
#ifndef PUGIHTML_DEPRECATED
#	if defined(__GNUC__)
#		define PUGIHTML_DEPRECATED __attribute__((deprecated))
#	elif defined(_MSC_VER) && _MSC_VER >= 1300
#		define PUGIHTML_DEPRECATED __declspec(deprecated)
#	else
#		define PUGIHTML_DEPRECATED
#	endif
#endif

// Include exception header for XPath
#if !defined(PUGIHTML_NO_XPATH) && !defined(PUGIHTML_NO_EXCEPTIONS)
#	include <exception>
#endif

// If no API is defined, assume default
#ifndef PUGIHTML_API
#   define PUGIHTML_API
#endif

// If no API for classes is defined, assume default
#ifndef PUGIHTML_CLASS
#   define PUGIHTML_CLASS PUGIHTML_API
#endif

// If no API for functions is defined, assume default
#ifndef PUGIHTML_FUNCTION
#   define PUGIHTML_FUNCTION PUGIHTML_API
#endif

// uintptr_t
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#	include <stdint.h>
#else
#	if _MSC_VER < 1300
// No native uintptr_t in MSVC6
typedef size_t uintptr_t;
#	endif
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef __int32 int32_t;
#endif

// Inlining controls
#if defined(_MSC_VER) && _MSC_VER >= 1300
#	define PUGIHTML_NO_INLINE __declspec(noinline)
#elif defined(__GNUC__)
#	define PUGIHTML_NO_INLINE __attribute__((noinline))
#else
#	define PUGIHTML_NO_INLINE 
#endif

// Character interface macros
#ifdef PUGIHTML_WCHAR_MODE
#	define PUGIHTML_TEXT(t) L ## t
#	define PUGIHTML_CHAR wchar_t
#else
#	define PUGIHTML_TEXT(t) t
#	define PUGIHTML_CHAR char
#endif

namespace pugihtml
{
	// Character type used for all internal storage and operations; depends on PUGIHTML_WCHAR_MODE
	typedef PUGIHTML_CHAR char_t;

#ifndef PUGIHTML_NO_STL
	// String type used for operations that work with STL string; depends on PUGIHTML_WCHAR_MODE
	typedef std::basic_string<PUGIHTML_CHAR, std::char_traits<PUGIHTML_CHAR>, std::allocator<PUGIHTML_CHAR> > string_t;
#endif
}

#endif
