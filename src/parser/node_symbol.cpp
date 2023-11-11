//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#include "node_symbol.h"

using namespace o2;

void node_symbol::test_collision()
{
	class visitor : public query_node_visitor
	{
	public:
		node_symbol* _symbol;

	public:
		explicit visitor(node_symbol* symbol)
				: _symbol(symbol)
		{
		}

		void visit(node* n) final
		{
			if (n == _symbol)
				return;

			const auto other_symbol = dynamic_cast<node_symbol*>(n);
			if (other_symbol)
			{
				if (_symbol->get_id() == other_symbol->get_id())
				{
					throw error_named_symbol_already_declared(_symbol->get_source_code(), _symbol->get_id());
				}
			}
		}
	} v(this);

	const auto parent = get_parent_of_type<node_symbol>();
	parent->query(&v, query_flag_children | query_flag_downwards);
}

bool node_symbol::is_allowed(node* n) const
{
	// TODO iterate upwards in order to figure out if this symbol is allowed to be accessed
	return true;
}

void collision_detector::add(node_symbol* c)
{
	_colliders.add(c);
}

void collision_detector::test()
{
	for (auto c: _colliders)
		c->test_collision();
}
