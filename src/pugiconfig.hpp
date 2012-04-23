/**
 * pugihtml parser - version 0.1
 * --------------------------------------------------------
 * Copyright (c) 2012 Adgooroo, LLC (kgantchev [AT] adgooroo [DOT] com)
 *
 * This library is distributed under the MIT License. See notice in license.txt
 *
 * This work is based on the pugxml parser, which is: 
 * Copyright (C) 2006-2010, by Arseny Kapoulkine (arseny [DOT] kapoulkine [AT] gmail [DOT] com)
 */


#ifndef HEADER_PUGICONFIG_HPP
#define HEADER_PUGICONFIG_HPP

// Uncomment this to enable wchar_t mode
// #define PUGIHTML_WCHAR_MODE

// Uncomment this to disable XPath
// #define PUGIHTML_NO_XPATH

// Uncomment this to disable STL
// Note: you can't use XPath with PUGIHTML_NO_STL
// #define PUGIHTML_NO_STL

// Uncomment this to disable exceptions
// Note: you can't use XPath with PUGIHTML_NO_EXCEPTIONS
// #define PUGIHTML_NO_EXCEPTIONS

// Set this to control attributes for public classes/functions, i.e.:
// #define PUGIHTML_API __declspec(dllexport) // to export all public symbols from DLL
// #define PUGIHTML_CLASS __declspec(dllimport) // to import all classes from DLL
// #define PUGIHTML_FUNCTION __fastcall // to set calling conventions to all public functions to fastcall
// In absence of PUGIHTML_CLASS/PUGIHTML_FUNCTION definitions PUGIHTML_API is used instead

#endif
