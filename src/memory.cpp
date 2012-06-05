#include "memory.hpp"
#include <assert.h>
#include <stddef.h>

namespace pugihtml
{ 

    html_memory_page* html_memory_page::construct(void* memory)
	{
		if (!memory) return 0; //$ redundant, left for performance

		html_memory_page* result = static_cast<html_memory_page*>(memory);

		result->allocator = 0;
		result->memory = 0;
		result->prev = 0;
		result->next = 0;
		result->busy_size = 0;
		result->freed_size = 0;

		return result;
	}
    
	html_allocator::html_allocator(html_memory_page* root): _root(root), _busy_size(root->busy_size)
	{
	}
    
    void html_allocator::deallocate_page(html_memory_page* page)
	{
		global_deallocate(page->memory);
	}


	html_memory_page* html_allocator::allocate_page(size_t data_size)
	{
		size_t size = offsetof(html_memory_page, data) + data_size;

		// allocate block with some alignment, leaving memory for worst-case padding
		void* memory = global_allocate(size + html_memory_page_alignment);
		if (!memory) return 0;

		// align upwards to page boundary
		void* page_memory = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(memory) + (html_memory_page_alignment - 1)) & ~(html_memory_page_alignment - 1));

		// prepare page structure
		html_memory_page* page = html_memory_page::construct(page_memory);

		page->memory = memory;
		page->allocator = _root->allocator;

		return page;
	}
    
	void* html_allocator::allocate_memory(size_t size, html_memory_page*& out_page)
	{
		if (_busy_size + size > html_memory_page_size) return allocate_memory_oob(size, out_page);

		void* buf = _root->data + _busy_size;

		_busy_size += size;

		out_page = _root;

		return buf;
	}

	void html_allocator::deallocate_memory(void* ptr, size_t size, html_memory_page* page)
	{
		if (page == _root) page->busy_size = _busy_size;

		assert(ptr >= page->data && ptr < page->data + page->busy_size);
		(void)!ptr;

		page->freed_size += size;
		assert(page->freed_size <= page->busy_size);

		if (page->freed_size == page->busy_size)
		{
			if (page->next == 0)
			{
				assert(_root == page);

				// top page freed, just reset sizes
				page->busy_size = page->freed_size = 0;
				_busy_size = 0;
			}
			else
			{
				assert(_root != page);
				assert(page->prev);

				// remove from the list
				page->prev->next = page->next;
				page->next->prev = page->prev;

				// deallocate
				deallocate_page(page);
			}
		}
	}

	char_t* html_allocator::allocate_string(size_t length)
	{
		// allocate memory for string and header block
		size_t size = sizeof(html_memory_string_header) + length * sizeof(char_t);
			
		// round size up to pointer alignment boundary
		size_t full_size = (size + (sizeof(void*) - 1)) & ~(sizeof(void*) - 1);

		html_memory_page* page;
		html_memory_string_header* header = static_cast<html_memory_string_header*>(allocate_memory(full_size, page));

		if (!header) return 0;

		// setup header
		ptrdiff_t page_offset = reinterpret_cast<char*>(header) - page->data;

		assert(page_offset >= 0 && page_offset < (1 << 16));
		header->page_offset = static_cast<uint16_t>(page_offset);

		// full_size == 0 for large strings that occupy the whole page
		assert(full_size < (1 << 16) || (page->busy_size == full_size && page_offset == 0));
		header->full_size = static_cast<uint16_t>(full_size < (1 << 16) ? full_size : 0);

		return reinterpret_cast<char_t*>(header + 1);
	}

	void html_allocator::deallocate_string(char_t* string)
	{
		// get header
		html_memory_string_header* header = reinterpret_cast<html_memory_string_header*>(string) - 1;

		// deallocate
		size_t page_offset = offsetof(html_memory_page, data) + header->page_offset;
		html_memory_page* page = reinterpret_cast<html_memory_page*>(reinterpret_cast<char*>(header) - page_offset);

		// if full_size == 0 then this string occupies the whole page
		size_t full_size = header->full_size == 0 ? page->busy_size : header->full_size;

		deallocate_memory(header, full_size, page);
	}

    
	PUGIHTML_NO_INLINE void* html_allocator::allocate_memory_oob(size_t size, html_memory_page*& out_page)
	{
		const size_t large_allocation_threshold = html_memory_page_size / 4;

		html_memory_page* page = allocate_page(size <= large_allocation_threshold ? html_memory_page_size : size);
		if (!page) return 0;

		if (size <= large_allocation_threshold)
		{
			_root->busy_size = _busy_size;

			// insert page at the end of linked list
			page->prev = _root;
			_root->next = page;
			_root = page;

			_busy_size = size;
		}
		else
		{
			// insert page before the end of linked list, so that it is deleted as soon as possible
			// the last page is not deleted even if it's empty (see deallocate_memory)
			assert(_root->prev);

			page->prev = _root->prev;
			page->next = _root;

			_root->prev->next = page;
			_root->prev = page;
		}

		// allocate inside page
		page->busy_size = size;

		out_page = page;
		return page->data;
	}
}
