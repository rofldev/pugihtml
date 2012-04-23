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

#ifndef PUGI_UTIL_H
#define PUGI_UTIL_H
#include <set>
#include "common.hpp"

namespace pugihtml
{
    //#define ARRAYSIZE(ar)  (sizeof(ar) / sizeof(ar[0]))
    #define TOUPPER(X){ if((X) >= 'a' && (X) <= 'z') {(X) -= ('a' - 'A');} }
    
    static inline void to_upper(char_t* str)
    {
        while(*str!=0)
        { 
            TOUPPER(*str);
            str++;
        }
    }

    //static char_t* attributes[] = {"ABBR", "ACCEPT", "ACCEPT-CHARSET", 
    //    "ACCESSKEY", "ACTION", "ALIGN", "ALINK", "ALT", "ARCHIVE", 
    //    "AXIS", "BACKGROUND", "BGCOLOR", "BORDER", "CELLPADDING", 
    //    "CELLSPACING", "CHAR", "CHAROFF", "CHARSET", "CHECKED", "CITE", 
    //    "CLASS", "CLASSID", "CLEAR", "CODE", "CODEBASE", "CODETYPE", 
    //    "COLOR", "COLS", "COLSPAN", "COMPACT", "CONTENT", "COORDS", 
    //    "DATA", "DATETIME", "DECLARE", "DEFER", "DIR", "DISABLED", 
    //    "ENCTYPE", "FACE", "FOR", "FRAME", "FRAMEBORDER", "HEADERS", 
    //    "HEIGHT", "HREF", "HREFLANG", "HSPACE", "HTTP-EQUIV", "ID", 
    //    "ISMAP", "LABEL", "LANG", "LANGUAGE", "LINK", "LONGDESC", 
    //    "MARGINHEIGHT", "MARGINWIDTH", "MAXLENGTH", "MEDIA", "METHOD", 
    //    "MULTIPLE", "NAME", "NOHREF", "NORESIZE", "NOSHADE", "NOWRAP", 
    //    "OBJECT", "ONBLUR", "ONCHANGE", "ONCLICK", "ONDBLCLICK", 
    //    "ONFOCUS", "ONKEYDOWN", "ONKEYPRESS", "ONKEYUP", "ONLOAD", 
    //    "ONMOUSEDOWN", "ONMOUSEMOVE", "ONMOUSEOUT", "ONMOUSEOVER", 
    //    "ONMOUSEUP", "ONRESET", "ONSELECT", "ONSUBMIT", "ONUNLOAD", 
    //    "PROFILE", "PROMPT", "READONLY", "REL", "REV", "ROWS", "ROWSPAN", 
    //    "RULES", "SCHEME", "SCOPE", "SCROLLING", "SELECTED", "SHAPE", 
    //    "SIZE", "SPAN", "SRC", "STANDBY", "START", "STYLE", "SUMMARY", 
    //    "TABINDEX", "TARGET", "TEXT", "TITLE", "TYPE", "USEMAP", "VALIGN", 
    //    "VALUE", "VALUETYPE", "VERSION", "VLINK", "VSPACE", "WIDTH"};

    //static char_t* elements[] = {"!DOCTYPE", "A", "ABBR", "ACRONYM", "ADDRESS", 
    //    "APPLET", "AREA", "ARTICLE", "ASIDE", "AUDIO", "B", "BASE", 
    //    "BASEFONT", "BDI", "BDO", "BIG", "BLOCKQUOTE", "BODY", "BR", 
    //    "BUTTON", "CANVAS", "CAPTION", "CENTER", "CITE", "CODE", "COL", 
    //    "COLGROUP", "COMMAND", "DATALIST", "DD", "DEL", "DETAILS", 
    //    "DFN", "DIR", "DIV", "DL", "DT", "EM", "EMBED", "FIELDSET", 
    //    "FIGCAPTION", "FIGURE", "FONT", "FOOTER", "FORM", "FRAME", 
    //    "FRAMESET", "H1> TO <H6", "HEAD", "HEADER", "HGROUP", "HR", 
    //    "HTML", "I", "IFRAME", "IMG", "INPUT", "INS", "KEYGEN", "KBD", 
    //    "LABEL", "LEGEND", "LI", "LINK", "MAP", "MARK", "MENU", "META", 
    //    "METER", "NAV", "NOFRAMES", "NOSCRIPT", "OBJECT", "OL", "OPTGROUP", 
    //    "OPTION", "OUTPUT", "P", "PARAM", "PRE", "PROGRESS", "Q", "RP", 
    //    "RT", "RUBY", "S", "SAMP", "SCRIPT", "SECTION", "SELECT", "SMALL", 
    //    "SOURCE", "SPAN", "STRIKE", "STRONG", "STYLE", "SUB", "SUMMARY", 
    //    "SUP", "TABLE", "TBODY", "TD", "TEXTAREA", "TFOOT", "TH", "THEAD", 
    //    "TIME", "TITLE", "TR", "TRACK", "TT", "U", "UL", "VAR", "VIDEO", "WBR"};
    //
    //
    ///// Comparator optimized for speed
    //struct TagSetComparator
    //{
    //    ///// A specialized comparison of two strings: the first string
    //    ///// should always be upper case (since we're using this in the
    //    ///// html tag and attribute sets).
    //    //static inline int strcasecmp(const char_t *s1, const char_t *s2)
    //    //{
    //    //    char_t c1, c2;
    //    //    do 
    //    //    {
    //    //        c1 = *s1++;
    //    //        c2 = *s2++;
    //    //        TOUPPER(c1);
    //    //        TOUPPER(c2);
    //    //    } while((c1 == c2) && (s1 != '\0'));
    //    //    return (int) c1-c2;
    //    //}

    //    inline bool operator()(const char_t *s1, const char_t *s2) const
    //    {
    //        return strcmp(s1, s2) < 0;
    //    }
    //};
    //
    ///// HTML5 attributes
    //static const std::set<char_t*, TagSetComparator> html_attributes(attributes, attributes+ARRAYSIZE(attributes));
    //
    ///// HTML5 elements
    //static const std::set<char_t*, TagSetComparator> html_elements(elements, elements+ARRAYSIZE(elements));

    ///// Tag normalization involves the capitalization of the tag if it 
    ///// was found in the given tag set. 
    /////
    ///// tag is the tag which should be normalized
    ///// 
    ///// tmp is a the temporary character array has to be large enough 
    ///// to store valid tags and the number of characters in the tag 
    ///// should not surpass the size of the temporary character array.
    ///// 
    ///// tagSet is the set of tags which the given tag should be 
    ///// checked against.
    //static inline void normalize(char_t* tag, char_t* tmp, const std::set<char_t*, TagSetComparator>& tagSet )
    //{
    //    strcpy(tmp, tag); 
    //    to_upper(tmp);
    //    if(tagSet.find(tmp) != tagSet.end())
    //    {
    //        strcpy(tag, tmp);
    //    }
    //    memset(tmp, 0, strlen(tmp));
    //}
}
#endif
