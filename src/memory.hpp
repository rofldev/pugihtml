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

#ifndef MEMORY_HPP
#define MEMORY_HPP
#include "common.hpp"
#include <stdlib.h>

namespace pugihtml
{
	static const size_t html_memory_page_size = 32768;
    static const uintptr_t html_memory_page_alignment = 32;
	static const uintptr_t html_memory_page_pointer_mask = ~(html_memory_page_alignment - 1);
	static const uintptr_t html_memory_page_name_allocated_mask = 16;
	static const uintptr_t html_memory_page_value_allocated_mask = 8;
	static const uintptr_t html_memory_page_type_mask = 7;
    
    // Memory allocation function interface; returns pointer to allocated memory or NULL on failure
	typedef void* (*allocation_function)(size_t size);
	
	// Memory deallocation function interface
    typedef void (*deallocation_function)(void* ptr);
    
    // Memory allocation
	inline void* default_allocate(size_t size)
	{
		return malloc(size);
	}
    // Memory deallocation
	inline void default_deallocate(void* ptr)
	{
		free(ptr);
	}
    
    static allocation_function global_allocate = default_allocate;
	static deallocation_function global_deallocate = default_deallocate;

    // Override default memory management functions. All subsequent allocations/deallocations will be performed via supplied functions.
    void PUGIHTML_FUNCTION set_memory_management_functions(allocation_function allocate, deallocation_function deallocate);
    
    // Get current memory management functions
    allocation_function PUGIHTML_FUNCTION get_memory_allocation_function();
    deallocation_function PUGIHTML_FUNCTION get_memory_deallocation_function();
    
	struct html_allocator;

	struct html_memory_page
	{
		static html_memory_page* construct(void* memory);

		html_allocator* allocator;

		void* memory;

		html_memory_page* prev;
		html_memory_page* next;

		size_t busy_size;
		size_t freed_size;

		char data[1];
	};

	struct html_memory_string_header
	{
		uint16_t page_offset; // offset from page->data
		uint16_t full_size; // 0 if string occupies whole page
	};

	struct html_allocator
	{
		html_allocator(html_memory_page* root);

		html_memory_page* allocate_page(size_t data_size);

		static void deallocate_page(html_memory_page* page);

		void* allocate_memory_oob(size_t size, html_memory_page*& out_page);

		void* allocate_memory(size_t size, html_memory_page*& out_page);

		void deallocate_memory(void* ptr, size_t size, html_memory_page* page);

		pugihtml::char_t* allocate_string(size_t length);

		void deallocate_string(pugihtml::char_t* string);

		html_memory_page* _root;
		size_t _busy_size;
	};

}
#endif
