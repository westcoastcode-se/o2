//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_import.h"
#include "module/node_module.h"
#include <iostream>

using namespace o2;

void node_import::debug(std::basic_ostream<char>& stream, int indent) const
{
	stream << this << in(indent);
	stream << "import(statement=" << _import_statement << ",package=" << _package;
	stream << ",alias=" << _alias << ")" << std::endl;
	node::debug(stream, indent);
}

bool node_import::resolve(const recursion_detector* rd)
{
	// resolve package first and then the rest of the children
	if (_package == nullptr)
	{
		class modules_visitor
				: public query_node_visitor
		{
		public:
			const string_view text;
			vector<node_module*> modules;

			modules_visitor(string_view text)
					: text(text)
			{
			}

			void visit(node* const n) final
			{
				const auto impl = dynamic_cast<node_module*>(n);
				if (impl && text.starts_with(impl->get_name()))
					modules.add(impl);
			}
		} v1(_import_statement);

		// Query modules upwards until we'll reach root then query down one step
		// this is because private packages are put as children under the module node
		get_parent()->query(&v1, node::query_flag_parents | node::query_flag_children_from_root);
		if (v1.modules.empty())
			throw std::runtime_error("could not find modules matching import statements");

		// look for packages in each module
		class package_visitor
				: public query_node_visitor
		{
		public:
			const string_view text;
			vector<node_package*>& packages;

			package_visitor(string_view text, vector<node_package*>& packages)
					: text(text), packages(packages)
			{
			}

			void visit(node* const n) final
			{
				const auto impl = dynamic_cast<node_package*>(n);
				if (impl && text.starts_with(impl->get_name()))
					packages.add(impl);
			}
		};

		vector<node_package*> packages;
		for (auto m: v1.modules)
		{
			package_visitor v2(m->get_module()->get_relative_path(_import_statement),
					packages);
			m->query(&v2, query_flag_children | query_flag_downwards);
			if (!packages.empty())
			{
				_package = packages[0];
				break;
			}
		}

		if (packages.empty())
			throw std::runtime_error("unresolved import statements");
		_package = packages[0];
		if (_package == nullptr)
			return false;
	}

	return node::resolve(rd);
}

void node_import::query(query_node_visitor* visitor, int flags)
{
	// If we allow searching for references then allow to search for the first child level in that reference
	if (bit_isset(flags, query_flag_follow_refs) && _package)
	{
		// Do not follow a ref's ref
		int new_flags = bit_unset(flags, query_flag_follow_refs);
		// Do not query the ref's parents
		new_flags = bit_unset(new_flags, query_flag_parents);
		// Search the ref's children
		new_flags = bit_set(new_flags, query_flag_children);

		_package->query(visitor, new_flags);
	}

	node::query(visitor, flags);
}
