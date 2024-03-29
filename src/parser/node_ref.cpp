//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_ref.h"
#include "package/node_package.h"
#include "types/node_type.h"
#include "types/node_type_primitive.h"
#include "types/complex/node_type_complex.h"
#include "node_import.h"
#include "functions/node_func.h"

using namespace o2;

void node_ref::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "ref(text=" << _text << ",";
	stream << "query=[";
	bool comma = false;
	for (int i = 0; i < 32; ++i)
	{
		const int bit = (1 << i);
		if (_query_types & bit)
		{
			if (comma)
				stream << ',';
			comma = true;
			switch (static_cast<node_ref::query_types>(bit))
			{
			case node_ref::package:
				stream << "package";
				break;
			case node_ref::type:
				stream << "type";
				break;
			case node_ref::func:
				stream << "func";
				break;
			case node_ref::method:
				stream << "method";
				break;
			case node_ref::arg:
				stream << "arg";
				break;
			case node_ref::local:
				stream << "local";
				break;
			case node_ref::global:
				stream << "global";
				break;
			case node_ref::primitive:
				stream << "primitive";
				break;
			case node_ref::const_:
				stream << "const";
				break;
			}
		}
	}
	stream << ']';
	stream << ",results=[";
	for (int i = 0; i < _results.size(); ++i)
	{
		if (i > 0)
			stream << ',';
		const auto result = _results[i];
		stream << result;
	}
	stream << "]";
	stream << ")" << std::endl;
	node::debug(stream, indent);
}

void node_ref::resolve0(const recursion_detector* rd, resolve_state* state)
{
	assert(_results.empty() && "expected the resolve method to be called only once");

	auto parent = get_parent();
	if (bit_isset(_query_flags, query_flag_from_root))
		parent = parent->get_root_node();

	resolve_from_parent(parent);

	auto leaf = this;
	while (true)
	{
		auto children = leaf->get_children();
		if (children.empty())
			break;
		leaf = static_cast<node_ref*>(leaf->get_child(0));
	}
	if (leaf->_results.empty())
		throw resolve_error_unresolved_reference(get_source_code());
	if (leaf == this)
		return;
	_results = leaf->_results;
	node::resolve0(rd, state);
}

bool node_ref::resolve_from_parent(node* parent)
{
	class visitor : public query_node_visitor
	{
	public:
		const int query;
		const string_view text;
		node_ref* const dest;

		visitor(int query, string_view text, node_ref* dest)
				: query(query), text(text), dest(dest)
		{
		}

		void visit(node* const n) final
		{
			if ((query & query_types::package) == query_types::package)
			{
				const auto impl = dynamic_cast<node_import*>(n);
				if (impl && impl->get_alias() == text)
				{
					add(impl->get_package());
					return;
				}
			}

			if ((query & query_types::primitive))
			{
				const auto impl = dynamic_cast<node_type_primitive*>(n);
				if (impl)
				{
					for (auto name: impl->get_names())
					{
						if (name == text)
						{
							add(impl);
							return;
						}
					}
				}
			}

			if ((query & (query_types::arg | query_types::local | query_types::global)) != 0)
			{
				const auto impl = dynamic_cast<node_var*>(n);
				if (impl && impl->get_name() == text)
				{
					add(impl);
					return;
				}
			}

			if ((query & query_types::type))
			{
				const auto impl = dynamic_cast<node_type_complex*>(n);
				if (impl && impl->get_name() == text)
				{
					add(impl);
					return;
				}
			}

			if ((query & query_types::func))
			{
				const auto impl = dynamic_cast<node_func*>(n);
				if (impl && impl->get_name() == text)
				{
					add(impl);
					return;
				}
			}
		}

		void add(node* n)
		{
			for (int i = 0; i < dest->_results.size(); ++i)
			{
				if (dest->_results[i] == n)
					return;
			}
			dest->_results.add(n);
		}
	} visitor(_query_types, _text, this);

	// search for references
	parent->query(&visitor, _query_flags);

	// could not resolve any references
	if (_results.empty())
		return false;

	for (auto child: get_children())
	{
		auto ref = static_cast<node_ref*>(child);
		for (auto result: _results)
		{
			ref->resolve_from_parent(result);
			ref->remove_phases_left(phase_resolve);
		}
	}
	return true;
}
