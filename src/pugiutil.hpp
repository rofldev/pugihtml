/**
 * pugihtml parser - version 0.1
 * --------------------------------------------------------
 * Copyright (C) 2012, by Kiril Gantchev (kgantchev [AT] gmail [DOT] com)
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is: 
 * Copyright (C) 2006-2010, by Arseny Kapoulkine (arseny [DOT] kapoulkine [AT] gmail [DOT] com)
 */

#ifndef PUGI_UTIL_H
#define PUGI_UTIL_H
#include "pugihtml.hpp"
#include <set>

namespace pugihtml
{
    #define ARRAYSIZE(ar)  (sizeof(ar) / sizeof(ar[0]))
    #define TOUPPER(X){ if((X) >= 'a' && (X) <= 'z') {(X) -= ('a' - 'A');} }
    
    static inline void to_upper(char_t* str)
    {
        while(*str!=0)
        { 
            TOUPPER(*str);
            str++;
        }
    }

    static char_t* attributes[] = {"ABBR", "ACCEPT", "ACCEPT-CHARSET", 
        "ACCESSKEY", "ACTION", "ALIGN", "ALINK", "ALT", "ARCHIVE", 
        "AXIS", "BACKGROUND", "BGCOLOR", "BORDER", "CELLPADDING", 
        "CELLSPACING", "CHAR", "CHAROFF", "CHARSET", "CHECKED", "CITE", 
        "CLASS", "CLASSID", "CLEAR", "CODE", "CODEBASE", "CODETYPE", 
        "COLOR", "COLS", "COLSPAN", "COMPACT", "CONTENT", "COORDS", 
        "DATA", "DATETIME", "DECLARE", "DEFER", "DIR", "DISABLED", 
        "ENCTYPE", "FACE", "FOR", "FRAME", "FRAMEBORDER", "HEADERS", 
        "HEIGHT", "HREF", "HREFLANG", "HSPACE", "HTTP-EQUIV", "ID", 
        "ISMAP", "LABEL", "LANG", "LANGUAGE", "LINK", "LONGDESC", 
        "MARGINHEIGHT", "MARGINWIDTH", "MAXLENGTH", "MEDIA", "METHOD", 
        "MULTIPLE", "NAME", "NOHREF", "NORESIZE", "NOSHADE", "NOWRAP", 
        "OBJECT", "ONBLUR", "ONCHANGE", "ONCLICK", "ONDBLCLICK", 
        "ONFOCUS", "ONKEYDOWN", "ONKEYPRESS", "ONKEYUP", "ONLOAD", 
        "ONMOUSEDOWN", "ONMOUSEMOVE", "ONMOUSEOUT", "ONMOUSEOVER", 
        "ONMOUSEUP", "ONRESET", "ONSELECT", "ONSUBMIT", "ONUNLOAD", 
        "PROFILE", "PROMPT", "READONLY", "REL", "REV", "ROWS", "ROWSPAN", 
        "RULES", "SCHEME", "SCOPE", "SCROLLING", "SELECTED", "SHAPE", 
        "SIZE", "SPAN", "SRC", "STANDBY", "START", "STYLE", "SUMMARY", 
        "TABINDEX", "TARGET", "TEXT", "TITLE", "TYPE", "USEMAP", "VALIGN", 
        "VALUE", "VALUETYPE", "VERSION", "VLINK", "VSPACE", "WIDTH"};

    static char_t* elements[] = {"!DOCTYPE", "A", "ABBR", "ACRONYM", "ADDRESS", 
        "APPLET", "AREA", "ARTICLE", "ASIDE", "AUDIO", "B", "BASE", 
        "BASEFONT", "BDI", "BDO", "BIG", "BLOCKQUOTE", "BODY", "BR", 
        "BUTTON", "CANVAS", "CAPTION", "CENTER", "CITE", "CODE", "COL", 
        "COLGROUP", "COMMAND", "DATALIST", "DD", "DEL", "DETAILS", 
        "DFN", "DIR", "DIV", "DL", "DT", "EM", "EMBED", "FIELDSET", 
        "FIGCAPTION", "FIGURE", "FONT", "FOOTER", "FORM", "FRAME", 
        "FRAMESET", "H1> TO <H6", "HEAD", "HEADER", "HGROUP", "HR", 
        "HTML", "I", "IFRAME", "IMG", "INPUT", "INS", "KEYGEN", "KBD", 
        "LABEL", "LEGEND", "LI", "LINK", "MAP", "MARK", "MENU", "META", 
        "METER", "NAV", "NOFRAMES", "NOSCRIPT", "OBJECT", "OL", "OPTGROUP", 
        "OPTION", "OUTPUT", "P", "PARAM", "PRE", "PROGRESS", "Q", "RP", 
        "RT", "RUBY", "S", "SAMP", "SCRIPT", "SECTION", "SELECT", "SMALL", 
        "SOURCE", "SPAN", "STRIKE", "STRONG", "STYLE", "SUB", "SUMMARY", 
        "SUP", "TABLE", "TBODY", "TD", "TEXTAREA", "TFOOT", "TH", "THEAD", 
        "TIME", "TITLE", "TR", "TRACK", "TT", "U", "UL", "VAR", "VIDEO", "WBR"};
    
    
    /// Comparator optimized for speed
    struct TagSetComparator
    {
        ///// A specialized comparison of two strings: the first string
        ///// should always be upper case (since we're using this in the
        ///// html tag and attribute sets).
        //static inline int strcasecmp(const char_t *s1, const char_t *s2)
        //{
        //    char_t c1, c2;
        //    do 
        //    {
        //        c1 = *s1++;
        //        c2 = *s2++;
        //        TOUPPER(c1);
        //        TOUPPER(c2);
        //    } while((c1 == c2) && (s1 != '\0'));
        //    return (int) c1-c2;
        //}

        inline bool operator()(const char_t *s1, const char_t *s2) const
        {
            return strcmp(s1, s2) < 0;
        }
    };
    
    /// HTML5 attributes
    static const std::set<char_t*, TagSetComparator> html_attributes(attributes, attributes+ARRAYSIZE(attributes));
    
    /// HTML5 elements
    static const std::set<char_t*, TagSetComparator> html_elements(elements, elements+ARRAYSIZE(elements));

    /// Tag normalization involves the capitalization of the tag if it 
    /// was found in the given tag set. 
    ///
    /// tag is the tag which should be normalized
    /// 
    /// tmp is a the temporary character array has to be large enough 
    /// to store valid tags and the number of characters in the tag 
    /// should not surpass the size of the temporary character array.
    /// 
    /// tagSet is the set of tags which the given tag should be 
    /// checked against.
    static inline void normalize(char_t* tag, char_t* tmp, const std::set<char_t*, TagSetComparator>& tagSet )
    {
        strcpy(tmp, tag); 
        to_upper(tmp);
        if(tagSet.find(tmp) != tagSet.end())
        {
            strcpy(tag, tmp);
        }
        memset(tmp, 0, strlen(tmp));
    }
}
//
//
//// Memory allocation
//namespace
//{
//	void* default_allocate(size_t size)
//	{
//		return malloc(size);
//	}
//
//	void default_deallocate(void* ptr)
//	{
//		free(ptr);
//	}
//
//	allocation_function global_allocate = default_allocate;
//	deallocation_function global_deallocate = default_deallocate;
//}
//
//// String utilities
//namespace
//{
//	// Get string length
//	size_t strlength(const char_t* s)
//	{
//		assert(s);
//
//	#ifdef PUGIHTML_WCHAR_MODE
//		return wcslen(s);
//	#else
//		return strlen(s);
//	#endif
//	}
//
//	// Compare two strings
//	bool strequal(const char_t* src, const char_t* dst)
//	{
//		assert(src && dst);
//
//	#ifdef PUGIHTML_WCHAR_MODE
//		return wcscmp(src, dst) == 0;
//	#else
//		return strcmp(src, dst) == 0;
//	#endif
//	}
//
//	// Compare lhs with [rhs_begin, rhs_end)
//	bool strequalrange(const char_t* lhs, const char_t* rhs, size_t count)
//	{
//		for (size_t i = 0; i < count; ++i)
//			if (lhs[i] != rhs[i])
//				return false;
//	
//		return lhs[count] == 0;
//	}
//	
//#ifdef PUGIHTML_WCHAR_MODE
//	// Convert string to wide string, assuming all symbols are ASCII
//	void widen_ascii(wchar_t* dest, const char* source)
//	{
//		for (const char* i = source; *i; ++i) *dest++ = *i;
//		*dest = 0;
//	}
//#endif
//}
//
//#if !defined(PUGIHTML_NO_STL) || !defined(PUGIHTML_NO_XPATH)
//// auto_ptr-like buffer holder for exception recovery
//namespace
//{
//	struct buffer_holder
//	{
//		void* data;
//		void (*deleter)(void*);
//
//		buffer_holder(void* data, void (*deleter)(void*)): data(data), deleter(deleter)
//		{
//		}
//
//		~buffer_holder()
//		{
//			if (data) deleter(data);
//		}
//
//		void* release()
//		{
//			void* result = data;
//			data = 0;
//			return result;
//		}
//	};
//}
//#endif

#endif


/**
 * Copyright (c) 2012 Kiril Gantchev
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */