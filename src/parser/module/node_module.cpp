//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_module.h"
#include "module.h"
#include "../package/node_package.h"

using namespace o2;

node_module::node_module(module* m)
		: node_symbol(source_code_view()), _name(m->get_name()), _module(m)
{
}

void node_module::debug(debug_ostream& stream, int indent) const
{
	stream << this << in(indent);
	stream << "module(name=" << _module->get_name() << ",path=" << _module->get_root_path() << ")" << std::endl;
	node::debug(stream, indent);
}

string_view node_module::get_version() const
{
	return _module->get_version();
}

string_view node_module::get_relative_path(string_view import_path) const
{
	return _module->get_relative_path(import_path);
}

void node_module::on_parent_node(node* p)
{
	test_collision(this);
}

void node_module::write_json_properties(json& j)
{
	j.write(json::pair<string_view>{ "type", "module" });
	node_symbol::write_json_properties(j);
	j.write(json::pair<string_view>{ "version", _module->get_version() });

	// add packages
	{
		auto package_json = j.write(json::array{ "packages" });
		for (auto package: get_children_of_type<node_package>())
		{
			package->write_json(package_json);
		}
	}
}

string node_module::get_id() const
{
	stringstream ss;
	ss << '/' << _module->get_name();
	if (!_module->get_version().empty())
	{
		ss << "@";
		ss << _module->get_version();
	}
	return std::move(ss.str());
}

bool node_module::compare_with_symbol(const node_module* rhs) const
{
	return get_name() == rhs->get_name() &&
		   get_version() == rhs->get_version();
}
