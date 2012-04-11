/*
 * Boost.Foreach support for pugihtml classes.
 * This file is provided to the public domain.
 * Written by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 */

#ifndef HEADER_PUGIHTML_FOREACH_HPP
#define HEADER_PUGIHTML_FOREACH_HPP

#include "pugihtml.hpp"

/*
 * These types add support for BOOST_FOREACH macro to html_node and html_document classes (child iteration only).
 * Example usage:
 * BOOST_FOREACH(html_node n, doc) {}
 */

namespace boost
{
	template <typename> struct range_mutable_iterator;
	template <typename> struct range_const_iterator;

	template<> struct range_mutable_iterator<pugi::html_node>
	{
		typedef pugi::html_node::iterator type;
	};

	template<> struct range_const_iterator<pugi::html_node>
	{
		typedef pugi::html_node::iterator type;
	};

	template<> struct range_mutable_iterator<pugi::html_document>
	{
		typedef pugi::html_document::iterator type;
	};

	template<> struct range_const_iterator<pugi::html_document>
	{
		typedef pugi::html_document::iterator type;
	};
}

/*
 * These types add support for BOOST_FOREACH macro to html_node and html_document classes (child/attribute iteration).
 * Example usage:
 * BOOST_FOREACH(html_node n, children(doc)) {}
 * BOOST_FOREACH(html_node n, attributes(doc)) {}
 */

namespace pugi
{
	struct html_node_children_adapter
	{
		typedef pugi::html_node::iterator iterator;
		typedef pugi::html_node::iterator const_iterator;

		html_node node;

		const_iterator begin() const
		{
			return node.begin();
		}

		const_iterator end() const
		{
			return node.end();
		}
	};

	html_node_children_adapter children(const pugi::html_node& node)
	{
		html_node_children_adapter result = {node};
		return result;
	}

	struct html_node_attribute_adapter
	{
		typedef pugi::html_node::attribute_iterator iterator;
		typedef pugi::html_node::attribute_iterator const_iterator;

		html_node node;

		const_iterator begin() const
		{
			return node.attributes_begin();
		}

		const_iterator end() const
		{
			return node.attributes_end();
		}
	};

	html_node_attribute_adapter attributes(const pugi::html_node& node)
	{
		html_node_attribute_adapter result = {node};
		return result;
	}
}

#endif
