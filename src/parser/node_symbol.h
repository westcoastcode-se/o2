//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"

namespace o2
{
	/**
	 * \brief base class for symbols that are not allowed to exist multiple times in the same scope
	 *
	 * all symbols have a unique id. For example a type might have a symbol id ".mycompany.MyType" or
	 * ".mycompany.MyMacroType<mycompany.Node>"
	 */
	class node_symbol
			: public node
	{
	public:
		node_symbol(const source_code_view& view)
				: node(view)
		{
		}

		/**
		 * \return a unique id that represents this symbol
		 */
		string_view get_id() const
		{
			return _id;
		}

		/**
		 * \brief test for collisions of this symbol
		 */
		void test_collision();

	protected:
		/**
		 * \param id the new id
		 */
		void set_id(string id)
		{
			_id = std::move(id);
		}

		/**
		 * \brief resolve the symbol's id
		 */
		virtual void resolve_symbol_id()
		{
		}

	private:
		string _id;
	};

	/**
	 * \brief detector used for checking collisions when adding nodes to the main syntax tree root
	 */
	class collision_detector
	{
	public:
		/**
		 * \brief add a symbol to be tested when we add it to the root syntax tree
		 * \param symbol
		 */
		void add(node_symbol* symbol);

		/**
		 * \brief test all symbols
		 */
		void test();

	private:
		vector<node_symbol*> _colliders;
	};
}
