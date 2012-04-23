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


#ifndef HEADER_PUGIHTML_HPP
#define HEADER_PUGIHTML_HPP

#include <stddef.h>
#include "common.hpp"

// The PugiHTML namespace
namespace pugihtml
{
	// Tree node types
	enum html_node_type
	{
		node_null,          // Empty (null) node handle
		node_document,		// A document tree's absolute root
		node_element,		// Element tag, i.e. '<node/>'
		node_pcdata,		// Plain character data, i.e. 'text'
		node_cdata,			// Character data, i.e. '<![CDATA[text]]>'
		node_comment,		// Comment tag, i.e. '<!-- text -->'
		node_pi,			// Processing instruction, i.e. '<?name?>'
		node_declaration,	// Document declaration, i.e. '<?html version="1.0"?>'
        node_doctype        // Document type declaration, i.e. '<!DOCTYPE doc>'
	};

	// Parsing options

	// Minimal parsing mode (equivalent to turning all other flags off).
    // Only elements and PCDATA sections are added to the DOM tree, no text conversions are performed.
	const unsigned int parse_minimal = 0x0000;

	// This flag determines if processing instructions (node_pi) are added to the DOM tree. This flag is off by default.
	const unsigned int parse_pi = 0x0001;

	// This flag determines if comments (node_comment) are added to the DOM tree. This flag is off by default.
	const unsigned int parse_comments = 0x0002;

	// This flag determines if CDATA sections (node_cdata) are added to the DOM tree. This flag is on by default.
	const unsigned int parse_cdata = 0x0004;

	// This flag determines if plain character data (node_pcdata) that consist only of whitespace are added to the DOM tree.
    // This flag is off by default; turning it on usually results in slower parsing and more memory consumption.
	const unsigned int parse_ws_pcdata = 0x0008;

	// This flag determines if character and entity references are expanded during parsing. This flag is on by default.
	const unsigned int parse_escapes = 0x0010;

	// This flag determines if EOL characters are normalized (converted to #xA) during parsing. This flag is on by default.
	const unsigned int parse_eol = 0x0020;
	
 	// This flag determines if attribute values are normalized using CDATA normalization rules during parsing. This flag is on by default.
 	const unsigned int parse_wconv_attribute = 0x0040;

 	// This flag determines if attribute values are normalized using NMTOKENS normalization rules during parsing. This flag is off by default.
 	const unsigned int parse_wnorm_attribute = 0x0080;
	
    // This flag determines if document declaration (node_declaration) is added to the DOM tree. This flag is off by default.
	const unsigned int parse_declaration = 0x0100;

    // This flag determines if document type declaration (node_doctype) is added to the DOM tree. This flag is off by default.
	const unsigned int parse_doctype = 0x0200;

	// The default parsing mode.
    // Elements, PCDATA and CDATA sections are added to the DOM tree, character/reference entities are expanded,
    // End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.
	const unsigned int parse_default = parse_cdata | parse_escapes | parse_wconv_attribute | parse_eol;

    // The full parsing mode.
    // Nodes of all types are added to the DOM tree, character/reference entities are expanded,
    // End-of-Line characters are normalized, attribute values are normalized using CDATA normalization rules.
    const unsigned int parse_full = parse_default | parse_pi | parse_comments | parse_declaration | parse_doctype;

	// These flags determine the encoding of input data for HTML document
	enum html_encoding
	{
		encoding_auto,      // Auto-detect input encoding using BOM or < / <? detection; use UTF8 if BOM is not found
		encoding_utf8,      // UTF8 encoding
		encoding_utf16_le,  // Little-endian UTF16
		encoding_utf16_be,  // Big-endian UTF16
		encoding_utf16,     // UTF16 with native endianness
		encoding_utf32_le,  // Little-endian UTF32
		encoding_utf32_be,  // Big-endian UTF32
		encoding_utf32,     // UTF32 with native endianness
		encoding_wchar      // The same encoding wchar_t has (either UTF16 or UTF32)
	};

	// Formatting flags
	
	// Indent the nodes that are written to output stream with as many indentation strings as deep the node is in DOM tree. This flag is on by default.
	const unsigned int format_indent = 0x01;
	
	// Write encoding-specific BOM to the output stream. This flag is off by default.
	const unsigned int format_write_bom = 0x02;

	// Use raw output mode (no indentation and no line breaks are written). This flag is off by default.
	const unsigned int format_raw = 0x04;
	
	// Omit default HTML declaration even if there is no declaration in the document. This flag is off by default.
	const unsigned int format_no_declaration = 0x08;

	// The default set of formatting flags.
    // Nodes are indented depending on their depth in DOM tree, a default declaration is output if document has none.
	const unsigned int format_default = format_indent;
		
	// Forward declarations
	struct html_attribute_struct;
	struct html_node_struct;

	class html_node_iterator;
	class html_attribute_iterator;

	class html_tree_walker;
	
	class html_node;

	#ifndef PUGIHTML_NO_XPATH
	class xpath_node;
	class xpath_node_set;
	class xpath_query;
	class xpath_variable_set;
	#endif

	// Writer interface for node printing (see html_node::print)
	class PUGIHTML_CLASS html_writer
	{
	public:
		virtual ~html_writer() {}

		// Write memory chunk into stream/file/whatever
		virtual void write(const void* data, size_t size) = 0;
	};

	// html_writer implementation for FILE*
	class PUGIHTML_CLASS html_writer_file: public html_writer
	{
	public:
        // Construct writer from a FILE* object; void* is used to avoid header dependencies on stdio
		html_writer_file(void* file);

		virtual void write(const void* data, size_t size);

	private:
		void* file;
	};

	#ifndef PUGIHTML_NO_STL
	// html_writer implementation for streams
	class PUGIHTML_CLASS html_writer_stream: public html_writer
	{
	public:
        // Construct writer from an output stream object
		html_writer_stream(std::basic_ostream<char, std::char_traits<char> >& stream);
		html_writer_stream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream);

		virtual void write(const void* data, size_t size);

	private:
		std::basic_ostream<char, std::char_traits<char> >* narrow_stream;
		std::basic_ostream<wchar_t, std::char_traits<wchar_t> >* wide_stream;
	};
	#endif

	// A light-weight handle for manipulating attributes in DOM tree
	class PUGIHTML_CLASS html_attribute
	{
		friend class html_attribute_iterator;
		friend class html_node;

	private:
		html_attribute_struct* _attr;
	
    	typedef html_attribute_struct* html_attribute::*unspecified_bool_type;

	public:
        // Default constructor. Constructs an empty attribute.
		html_attribute();
		
        // Constructs attribute from internal pointer
		explicit html_attribute(html_attribute_struct* attr);

    	// Safe bool conversion operator
    	operator unspecified_bool_type() const;

    	// Borland C++ workaround
    	bool operator!() const;

		// Comparison operators (compares wrapped attribute pointers)
		bool operator==(const html_attribute& r) const;
		bool operator!=(const html_attribute& r) const;
		bool operator<(const html_attribute& r) const;
		bool operator>(const html_attribute& r) const;
		bool operator<=(const html_attribute& r) const;
		bool operator>=(const html_attribute& r) const;

		// Check if attribute is empty
		bool empty() const;

		// Get attribute name/value, or "" if attribute is empty
		const char_t* name() const;
		const char_t* value() const;

		// Get attribute value as a number, or 0 if conversion did not succeed or attribute is empty
		int as_int() const;
		unsigned int as_uint() const;
		double as_double() const;
		float as_float() const;

        // Get attribute value as bool (returns true if first character is in '1tTyY' set), or false if attribute is empty
		bool as_bool() const;

        // Set attribute name/value (returns false if attribute is empty or there is not enough memory)
		bool set_name(const char_t* rhs);
		bool set_value(const char_t* rhs);

        // Set attribute value with type conversion (numbers are converted to strings, boolean is converted to "true"/"false")
		bool set_value(int rhs);
		bool set_value(unsigned int rhs);
		bool set_value(double rhs);
		bool set_value(bool rhs);

		// Set attribute value (equivalent to set_value without error checking)
		html_attribute& operator=(const char_t* rhs);
		html_attribute& operator=(int rhs);
		html_attribute& operator=(unsigned int rhs);
		html_attribute& operator=(double rhs);
		html_attribute& operator=(bool rhs);

        // Get next/previous attribute in the attribute list of the parent node
    	html_attribute next_attribute() const;
    	html_attribute previous_attribute() const;

        // Get hash value (unique for handles to the same object)
        size_t hash_value() const;

		// Get internal pointer
		html_attribute_struct* internal_object() const;
	};

#ifdef __BORLANDC__
	// Borland C++ workaround
	bool PUGIHTML_FUNCTION operator&&(const html_attribute& lhs, bool rhs);
	bool PUGIHTML_FUNCTION operator||(const html_attribute& lhs, bool rhs);
#endif

	// A light-weight handle for manipulating nodes in DOM tree
	class PUGIHTML_CLASS html_node
	{
		friend class html_attribute_iterator;
		friend class html_node_iterator;

	protected:
		html_node_struct* _root;

    	typedef html_node_struct* html_node::*unspecified_bool_type;

	public:
		// Default constructor. Constructs an empty node.
		html_node();

        // Constructs node from internal pointer
		explicit html_node(html_node_struct* p);

    	// Safe bool conversion operator
		operator unspecified_bool_type() const;

		// Borland C++ workaround
		bool operator!() const;
	
		// Comparison operators (compares wrapped node pointers)
		bool operator==(const html_node& r) const;
		bool operator!=(const html_node& r) const;
		bool operator<(const html_node& r) const;
		bool operator>(const html_node& r) const;
		bool operator<=(const html_node& r) const;
		bool operator>=(const html_node& r) const;

		// Check if node is empty.
		bool empty() const;

		// Get node type
		html_node_type type() const;

		// Get node name/value, or "" if node is empty or it has no name/value
		const char_t* name() const;
		const char_t* value() const;
	
		// Get attribute list
		html_attribute first_attribute() const;
        html_attribute last_attribute() const;

        // Get children list
		html_node first_child() const;
        html_node last_child() const;

        // Get next/previous sibling in the children list of the parent node
		html_node next_sibling() const;
		html_node previous_sibling() const;
		
        // Get parent node
		html_node parent() const;

		// Get root of DOM tree this node belongs to
		html_node root() const;

		// Get child, attribute or next/previous sibling with the specified name
		html_node child(const char_t* name) const;
		html_attribute attribute(const char_t* name) const;
		html_node next_sibling(const char_t* name) const;
		html_node previous_sibling(const char_t* name) const;

		// Get child value of current node; that is, value of the first child node of type PCDATA/CDATA
		const char_t* child_value() const;

		// Get child value of child with specified name. Equivalent to child(name).child_value().
		const char_t* child_value(const char_t* name) const;

		// Set node name/value (returns false if node is empty, there is not enough memory, or node can not have name/value)
		bool set_name(const char_t* rhs);
		bool set_value(const char_t* rhs);
		
		// Add attribute with specified name. Returns added attribute, or empty attribute on errors.
		html_attribute append_attribute(const char_t* name);
		html_attribute prepend_attribute(const char_t* name);
		html_attribute insert_attribute_after(const char_t* name, const html_attribute& attr);
		html_attribute insert_attribute_before(const char_t* name, const html_attribute& attr);

		// Add a copy of the specified attribute. Returns added attribute, or empty attribute on errors.
		html_attribute append_copy(const html_attribute& proto);
		html_attribute prepend_copy(const html_attribute& proto);
		html_attribute insert_copy_after(const html_attribute& proto, const html_attribute& attr);
		html_attribute insert_copy_before(const html_attribute& proto, const html_attribute& attr);

		// Add child node with specified type. Returns added node, or empty node on errors.
		html_node append_child(html_node_type type = node_element);
		html_node prepend_child(html_node_type type = node_element);
		html_node insert_child_after(html_node_type type, const html_node& node);
		html_node insert_child_before(html_node_type type, const html_node& node);

		// Add child element with specified name. Returns added node, or empty node on errors.
		html_node append_child(const char_t* name);
		html_node prepend_child(const char_t* name);
		html_node insert_child_after(const char_t* name, const html_node& node);
		html_node insert_child_before(const char_t* name, const html_node& node);

		// Add a copy of the specified node as a child. Returns added node, or empty node on errors.
		html_node append_copy(const html_node& proto);
		html_node prepend_copy(const html_node& proto);
		html_node insert_copy_after(const html_node& proto, const html_node& node);
		html_node insert_copy_before(const html_node& proto, const html_node& node);

		// Remove specified attribute
		bool remove_attribute(const html_attribute& a);
		bool remove_attribute(const char_t* name);

		// Remove specified child
		bool remove_child(const html_node& n);
		bool remove_child(const char_t* name);

		// Find attribute using predicate. Returns first attribute for which predicate returned true.
		template <typename Predicate> html_attribute find_attribute(Predicate pred) const
		{
			if (!_root) return html_attribute();
			
			for (html_attribute attrib = first_attribute(); attrib; attrib = attrib.next_attribute())
				if (pred(attrib))
					return attrib;
		
			return html_attribute();
		}

		// Find child node using predicate. Returns first child for which predicate returned true.
		template <typename Predicate> html_node find_child(Predicate pred) const
		{
			if (!_root) return html_node();
	
			for (html_node node = first_child(); node; node = node.next_sibling())
				if (pred(node))
					return node;
        
	        return html_node();
		}

		// Find node from subtree using predicate. Returns first node from subtree (depth-first), for which predicate returned true.
		template <typename Predicate> html_node find_node(Predicate pred) const
		{
			if (!_root) return html_node();

			html_node cur = first_child();
			
			while (cur._root && cur._root != _root)
			{
				if (pred(cur)) return cur;

				if (cur.first_child()) cur = cur.first_child();
				else if (cur.next_sibling()) cur = cur.next_sibling();
				else
				{
					while (!cur.next_sibling() && cur._root != _root) cur = cur.parent();

					if (cur._root != _root) cur = cur.next_sibling();
				}
			}

			return html_node();
		}

		// Find child node by attribute name/value
		html_node find_child_by_attribute(const char_t* name, const char_t* attr_name, const char_t* attr_value) const;
		html_node find_child_by_attribute(const char_t* attr_name, const char_t* attr_value) const;

	#ifndef PUGIHTML_NO_STL
		// Get the absolute node path from root as a text string.
		string_t path(char_t delimiter = '/') const;
	#endif

		// Search for a node by path consisting of node names and . or .. elements.
		html_node first_element_by_path(const char_t* path, char_t delimiter = '/') const;

		// Recursively traverse subtree with html_tree_walker
		bool traverse(html_tree_walker& walker);
	
	#ifndef PUGIHTML_NO_XPATH
		// Select single node by evaluating XPath query. Returns first node from the resulting node set.
		xpath_node select_single_node(const char_t* query, xpath_variable_set* variables = 0) const;
		xpath_node select_single_node(const xpath_query& query) const;

		// Select node set by evaluating XPath query
		xpath_node_set select_nodes(const char_t* query, xpath_variable_set* variables = 0) const;
		xpath_node_set select_nodes(const xpath_query& query) const;
	#endif
		
		// Print subtree using a writer object
		void print(html_writer& writer, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, html_encoding encoding = encoding_auto, unsigned int depth = 0) const;

	#ifndef PUGIHTML_NO_STL
		// Print subtree to stream
		void print(std::basic_ostream<char, std::char_traits<char> >& os, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, html_encoding encoding = encoding_auto, unsigned int depth = 0) const;
		void print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, unsigned int depth = 0) const;
	#endif

		// Child nodes iterators
		typedef html_node_iterator iterator;

		iterator begin() const;
		iterator end() const;

		// Attribute iterators
		typedef html_attribute_iterator attribute_iterator;

		attribute_iterator attributes_begin() const;
		attribute_iterator attributes_end() const;

		// Get node offset in parsed file/string (in char_t units) for debugging purposes
		ptrdiff_t offset_debug() const;

        // Get hash value (unique for handles to the same object)
        size_t hash_value() const;

		// Get internal pointer
		html_node_struct* internal_object() const;
	};

#ifdef __BORLANDC__
	// Borland C++ workaround
	bool PUGIHTML_FUNCTION operator&&(const html_node& lhs, bool rhs);
	bool PUGIHTML_FUNCTION operator||(const html_node& lhs, bool rhs);
#endif

	// Child node iterator (a bidirectional iterator over a collection of html_node)
	class PUGIHTML_CLASS html_node_iterator
	{
		friend class html_node;

	private:
		html_node _wrap;
		html_node _parent;

		html_node_iterator(html_node_struct* ref, html_node_struct* parent);

	public:
		// Iterator traits
		typedef ptrdiff_t difference_type;
		typedef html_node value_type;
		typedef html_node* pointer;
		typedef html_node& reference;

	#ifndef PUGIHTML_NO_STL
		typedef std::bidirectional_iterator_tag iterator_category;
	#endif

        // Default constructor
		html_node_iterator();

        // Construct an iterator which points to the specified node
		html_node_iterator(const html_node& node);

        // Iterator operators
		bool operator==(const html_node_iterator& rhs) const;
		bool operator!=(const html_node_iterator& rhs) const;

		html_node& operator*();
		html_node* operator->();

		const html_node_iterator& operator++();
		html_node_iterator operator++(int);

		const html_node_iterator& operator--();
		html_node_iterator operator--(int);
	};

	// Attribute iterator (a bidirectional iterator over a collection of html_attribute)
	class PUGIHTML_CLASS html_attribute_iterator
	{
		friend class html_node;

	private:
		html_attribute _wrap;
		html_node _parent;

		html_attribute_iterator(html_attribute_struct* ref, html_node_struct* parent);

	public:
		// Iterator traits
		typedef ptrdiff_t difference_type;
		typedef html_attribute value_type;
		typedef html_attribute* pointer;
		typedef html_attribute& reference;

	#ifndef PUGIHTML_NO_STL
		typedef std::bidirectional_iterator_tag iterator_category;
	#endif

        // Default constructor
		html_attribute_iterator();

        // Construct an iterator which points to the specified attribute
		html_attribute_iterator(const html_attribute& attr, const html_node& parent);

		// Iterator operators
		bool operator==(const html_attribute_iterator& rhs) const;
		bool operator!=(const html_attribute_iterator& rhs) const;

		html_attribute& operator*();
		html_attribute* operator->();

		const html_attribute_iterator& operator++();
		html_attribute_iterator operator++(int);

		const html_attribute_iterator& operator--();
		html_attribute_iterator operator--(int);
	};

	// Abstract tree walker class (see html_node::traverse)
	class PUGIHTML_CLASS html_tree_walker
	{
		friend class html_node;

	private:
		int _depth;
	
	protected:
		// Get current traversal depth
		int depth() const;
	
	public:
		html_tree_walker();
		virtual ~html_tree_walker();

		// Callback that is called when traversal begins
		virtual bool begin(html_node& node);

		// Callback that is called for each node traversed
		virtual bool for_each(html_node& node) = 0;

		// Callback that is called when traversal ends
		virtual bool end(html_node& node);
	};

	// Parsing status, returned as part of html_parse_result object
	enum html_parse_status
	{
		status_ok = 0,              // No error

		status_file_not_found,      // File was not found during load_file()
		status_io_error,            // Error reading from file/stream
		status_out_of_memory,       // Could not allocate memory
		status_internal_error,      // Internal error occurred

		status_unrecognized_tag,    // Parser could not determine tag type

		status_bad_pi,              // Parsing error occurred while parsing document declaration/processing instruction
		status_bad_comment,         // Parsing error occurred while parsing comment
		status_bad_cdata,           // Parsing error occurred while parsing CDATA section
		status_bad_doctype,         // Parsing error occurred while parsing document type declaration
		status_bad_pcdata,          // Parsing error occurred while parsing PCDATA section
		status_bad_start_element,   // Parsing error occurred while parsing start element tag
		status_bad_attribute,       // Parsing error occurred while parsing element attribute
		status_bad_end_element,     // Parsing error occurred while parsing end element tag
		status_end_element_mismatch // There was a mismatch of start-end tags (closing tag had incorrect name, some tag was not closed or there was an excessive closing tag)
	};

	// Parsing result
	struct PUGIHTML_CLASS html_parse_result
	{
		// Parsing status (see html_parse_status)
		html_parse_status status;

		// Last parsed offset (in char_t units from start of input data)
		ptrdiff_t offset;

		// Source document encoding
		html_encoding encoding;

        // Default constructor, initializes object to failed state
		html_parse_result();

		// Cast to bool operator
		operator bool() const;

		// Get error description
		const char* description() const;
	};

	// Document class (DOM tree root)
	class PUGIHTML_CLASS html_document: public html_node
	{
	private:
		char_t* _buffer;

		char _memory[192];
		
		// Non-copyable semantics
		html_document(const html_document&);
		const html_document& operator=(const html_document&);

		void create();
		void destroy();

		html_parse_result load_buffer_impl(void* contents, size_t size, unsigned int options, html_encoding encoding, bool is_mutable, bool own);

	public:
		// Default constructor, makes empty document
		html_document();

		// Destructor, invalidates all node/attribute handles to this document
		~html_document();

        // Removes all nodes, leaving the empty document
		void reset();

        // Removes all nodes, then copies the entire contents of the specified document
		void reset(const html_document& proto);

	#ifndef PUGIHTML_NO_STL
		// Load document from stream.
		html_parse_result load(std::basic_istream<char, std::char_traits<char> >& stream, unsigned int options = parse_default, html_encoding encoding = encoding_auto);
		html_parse_result load(std::basic_istream<wchar_t, std::char_traits<wchar_t> >& stream, unsigned int options = parse_default);
	#endif

		// Load document from zero-terminated string. No encoding conversions are applied.
		html_parse_result load(const char_t* contents, unsigned int options = parse_default);

		// Load document from file
		html_parse_result load_file(const char* path, unsigned int options = parse_default, html_encoding encoding = encoding_auto);
		html_parse_result load_file(const wchar_t* path, unsigned int options = parse_default, html_encoding encoding = encoding_auto);

		// Load document from buffer. Copies/converts the buffer, so it may be deleted or changed after the function returns.
		html_parse_result load_buffer(const void* contents, size_t size, unsigned int options = parse_default, html_encoding encoding = encoding_auto);

		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
        // You should ensure that buffer data will persist throughout the document's lifetime, and free the buffer memory manually once document is destroyed.
		html_parse_result load_buffer_inplace(void* contents, size_t size, unsigned int options = parse_default, html_encoding encoding = encoding_auto);

		// Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).
        // You should allocate the buffer with pugihtml allocation function; document will free the buffer when it is no longer needed (you can't use it anymore).
		html_parse_result load_buffer_inplace_own(void* contents, size_t size, unsigned int options = parse_default, html_encoding encoding = encoding_auto);

		// Save HTML document to writer (semantics is slightly different from html_node::print, see documentation for details).
		void save(html_writer& writer, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, html_encoding encoding = encoding_auto) const;

	#ifndef PUGIHTML_NO_STL
		// Save HTML document to stream (semantics is slightly different from html_node::print, see documentation for details).
		void save(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, html_encoding encoding = encoding_auto) const;
		void save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default) const;
	#endif

		// Save HTML to file
		bool save_file(const char* path, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, html_encoding encoding = encoding_auto) const;
		bool save_file(const wchar_t* path, const char_t* indent = PUGIHTML_TEXT("\t"), unsigned int flags = format_default, html_encoding encoding = encoding_auto) const;

        // Get document element
        html_node document_element() const;
	};

#ifndef PUGIHTML_NO_XPATH
	// XPath query return type
	enum xpath_value_type
	{
		xpath_type_none,      // Unknown type (query failed to compile)
		xpath_type_node_set,  // Node set (xpath_node_set)
		xpath_type_number,    // Number
		xpath_type_string,    // String
		xpath_type_boolean    // Boolean
	};

    // XPath parsing result
	struct PUGIHTML_CLASS xpath_parse_result
	{
		// Error message (0 if no error)
		const char* error;

		// Last parsed offset (in char_t units from string start)
		ptrdiff_t offset;

        // Default constructor, initializes object to failed state
		xpath_parse_result();

		// Cast to bool operator
		operator bool() const;

		// Get error description
		const char* description() const;
	};

	// A single XPath variable
	class PUGIHTML_CLASS xpath_variable
	{
		friend class xpath_variable_set;

	protected:
		xpath_value_type _type;
		xpath_variable* _next;

		xpath_variable();

		// Non-copyable semantics
		xpath_variable(const xpath_variable&);
		xpath_variable& operator=(const xpath_variable&);
		
	public:
        // Get variable name
		const char_t* name() const;

        // Get variable type
		xpath_value_type type() const;

        // Get variable value; no type conversion is performed, default value (false, NaN, empty string, empty node set) is returned on type mismatch error
		bool get_boolean() const;
		double get_number() const;
		const char_t* get_string() const;
		const xpath_node_set& get_node_set() const;

        // Set variable value; no type conversion is performed, false is returned on type mismatch error
		bool set(bool value);
		bool set(double value);
		bool set(const char_t* value);
		bool set(const xpath_node_set& value);
	};

	// A set of XPath variables
	class PUGIHTML_CLASS xpath_variable_set
	{
	private:
		xpath_variable* _data[64];

		// Non-copyable semantics
		xpath_variable_set(const xpath_variable_set&);
		xpath_variable_set& operator=(const xpath_variable_set&);

		xpath_variable* find(const char_t* name) const;

	public:
        // Default constructor/destructor
		xpath_variable_set();
		~xpath_variable_set();

        // Add a new variable or get the existing one, if the types match
		xpath_variable* add(const char_t* name, xpath_value_type type);

        // Set value of an existing variable; no type conversion is performed, false is returned if there is no such variable or if types mismatch
		bool set(const char_t* name, bool value);
		bool set(const char_t* name, double value);
		bool set(const char_t* name, const char_t* value);
		bool set(const char_t* name, const xpath_node_set& value);

        // Get existing variable by name
		xpath_variable* get(const char_t* name);
		const xpath_variable* get(const char_t* name) const;
	};

	// A compiled XPath query object
	class PUGIHTML_CLASS xpath_query
	{
	private:
		void* _impl;
		xpath_parse_result _result;

    	typedef void* xpath_query::*unspecified_bool_type;

		// Non-copyable semantics
		xpath_query(const xpath_query&);
		xpath_query& operator=(const xpath_query&);

	public:
        // Construct a compiled object from XPath expression.
        // If PUGIHTML_NO_EXCEPTIONS is not defined, throws xpath_exception on compilation errors.
		explicit xpath_query(const char_t* query, xpath_variable_set* variables = 0);

		// Destructor
		~xpath_query();

		// Get query expression return type
		xpath_value_type return_type() const;
		
		// Evaluate expression as boolean value in the specified context; performs type conversion if necessary.
        // If PUGIHTML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors.
		bool evaluate_boolean(const xpath_node& n) const;
		
		// Evaluate expression as double value in the specified context; performs type conversion if necessary.
        // If PUGIHTML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors.
		double evaluate_number(const xpath_node& n) const;
		
	#ifndef PUGIHTML_NO_STL
		// Evaluate expression as string value in the specified context; performs type conversion if necessary.
        // If PUGIHTML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors.
		string_t evaluate_string(const xpath_node& n) const;
	#endif
		
		// Evaluate expression as string value in the specified context; performs type conversion if necessary.
        // At most capacity characters are written to the destination buffer, full result size is returned (includes terminating zero).
        // If PUGIHTML_NO_EXCEPTIONS is not defined, throws std::bad_alloc on out of memory errors.
        // If PUGIHTML_NO_EXCEPTIONS is defined, returns empty  set instead.
		size_t evaluate_string(char_t* buffer, size_t capacity, const xpath_node& n) const;

		// Evaluate expression as node set in the specified context.
        // If PUGIHTML_NO_EXCEPTIONS is not defined, throws xpath_exception on type mismatch and std::bad_alloc on out of memory errors.
        // If PUGIHTML_NO_EXCEPTIONS is defined, returns empty node set instead.
		xpath_node_set evaluate_node_set(const xpath_node& n) const;

		// Get parsing result (used to get compilation errors in PUGIHTML_NO_EXCEPTIONS mode)
		const xpath_parse_result& result() const;

		// Safe bool conversion operator
		operator unspecified_bool_type() const;

    	// Borland C++ workaround
		bool operator!() const;
	};
	
	#ifndef PUGIHTML_NO_EXCEPTIONS
	// XPath exception class
	class PUGIHTML_CLASS xpath_exception: public std::exception
	{
	private:
		xpath_parse_result _result;

	public:
		// Construct exception from parse result
		explicit xpath_exception(const xpath_parse_result& result);

		// Get error message
		virtual const char* what() const throw();

        // Get parse result
		const xpath_parse_result& result() const;
	};
	#endif
	
	// XPath node class (either html_node or html_attribute)
	class PUGIHTML_CLASS xpath_node
	{
	private:
		html_node _node;
		html_attribute _attribute;
	
    	typedef html_node xpath_node::*unspecified_bool_type;

	public:
		// Default constructor; constructs empty XPath node
		xpath_node();
		
		// Construct XPath node from HTML node/attribute
		xpath_node(const html_node& node);
		xpath_node(const html_attribute& attribute, const html_node& parent);

		// Get node/attribute, if any
		html_node node() const;
		html_attribute attribute() const;
		
		// Get parent of contained node/attribute
		html_node parent() const;

    	// Safe bool conversion operator
		operator unspecified_bool_type() const;
		
    	// Borland C++ workaround
    	bool operator!() const;

		// Comparison operators
		bool operator==(const xpath_node& n) const;
		bool operator!=(const xpath_node& n) const;
	};

#ifdef __BORLANDC__
	// Borland C++ workaround
	bool PUGIHTML_FUNCTION operator&&(const xpath_node& lhs, bool rhs);
	bool PUGIHTML_FUNCTION operator||(const xpath_node& lhs, bool rhs);
#endif

	// A fixed-size collection of XPath nodes
	class PUGIHTML_CLASS xpath_node_set
	{
	public:
		// Collection type
		enum type_t
		{
			type_unsorted,			// Not ordered
			type_sorted,			// Sorted by document order (ascending)
			type_sorted_reverse		// Sorted by document order (descending)
		};
		
		// Constant iterator type
		typedef const xpath_node* const_iterator;
	
		// Default constructor. Constructs empty set.
		xpath_node_set();

		// Constructs a set from iterator range; data is not checked for duplicates and is not sorted according to provided type, so be careful
		xpath_node_set(const_iterator begin, const_iterator end, type_t type = type_unsorted);

		// Destructor
		~xpath_node_set();
		
		// Copy constructor/assignment operator
		xpath_node_set(const xpath_node_set& ns);
		xpath_node_set& operator=(const xpath_node_set& ns);

		// Get collection type
		type_t type() const;
		
		// Get collection size
		size_t size() const;

        // Indexing operator
		const xpath_node& operator[](size_t index) const;
		
		// Collection iterators
		const_iterator begin() const;
		const_iterator end() const;

		// Sort the collection in ascending/descending order by document order
		void sort(bool reverse = false);
		
		// Get first node in the collection by document order
		xpath_node first() const;
		
		// Check if collection is empty
		bool empty() const;
    
	private:
		type_t _type;
		
		xpath_node _storage;
		
		xpath_node* _begin;
		xpath_node* _end;

		void _assign(const_iterator begin, const_iterator end);
	};
#endif

#ifndef PUGIHTML_NO_STL
	// Convert wide string to UTF8
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > PUGIHTML_FUNCTION as_utf8(const wchar_t* str);
	std::basic_string<char, std::char_traits<char>, std::allocator<char> > PUGIHTML_FUNCTION as_utf8(const std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& str);
	
	// Convert UTF8 to wide string
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > PUGIHTML_FUNCTION as_wide(const char* str);
	std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > PUGIHTML_FUNCTION as_wide(const std::basic_string<char, std::char_traits<char>, std::allocator<char> >& str);
#endif
}

#if !defined(PUGIHTML_NO_STL) && (defined(_MSC_VER) || defined(__ICC))
namespace std
{
	// Workarounds for (non-standard) iterator category detection for older versions (MSVC7/IC8 and earlier)
	std::bidirectional_iterator_tag PUGIHTML_FUNCTION _Iter_cat(const pugihtml::html_node_iterator&);
	std::bidirectional_iterator_tag PUGIHTML_FUNCTION _Iter_cat(const pugihtml::html_attribute_iterator&);
}
#endif

#if !defined(PUGIHTML_NO_STL) && defined(__SUNPRO_CC)
namespace std
{
	// Workarounds for (non-standard) iterator category detection
	std::bidirectional_iterator_tag PUGIHTML_FUNCTION __iterator_category(const pugihtml::html_node_iterator&);
	std::bidirectional_iterator_tag PUGIHTML_FUNCTION __iterator_category(const pugihtml::html_attribute_iterator&);
}
#endif

#endif
