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
		enum accessor
		{
			// allow access to this symbol from only the current module
			accessor_module = 1,
			// allow anyone from accessing this symbol
			accessor_public,
			// only allow access from the same package
			accessor_private
		};

		/**
		 * \param view
		 */
		explicit node_symbol(const source_code_view& view)
				: node_symbol(view, accessor_public)
		{
		}

		/**
		 * \param view
		 * \param accessor whom are allowed to access this symbol
		 *
		 * TODO make sure to use the appropriate accessor for each symbol
		 */
		node_symbol(const source_code_view& view, accessor accessor)
				: node(view), _accessor(accessor)
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
		 * \return whom are allowed to access this symbol
		 */
		accessor get_accessor() const
		{
			return _accessor;
		}

		/**
		 * \return true if the current node is allowed to access this symbol
		 */
		bool is_allowed(node* n) const;

		bool is_module_access() const
		{
			return _accessor == accessor_module;
		}

		bool is_public() const
		{
			return _accessor == accessor_public;
		}

		bool is_private() const
		{
			return _accessor == accessor_private;
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
		accessor _accessor;
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
