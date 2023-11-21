//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"

namespace o2
{
	/**
	 * \brief helper type used when searching for collisions
	 * \tparam T
	 */
	template<class T>
	class node_symbol_collision_visitor final
			: public query_node_visitor
	{
	public:
		const T* const _this;

		explicit node_symbol_collision_visitor(const T* t)
				: _this(t)
		{
		}

		void visit(node* n) final
		{
			if (n == _this)
				return;

			const auto tt = dynamic_cast<const T*>(n);
			if (tt == nullptr)
				return;

			if (_this->compare_with_symbol(tt))
				throw error_named_symbol_already_declared(tt->get_source_code(), tt->get_name());
		}
	};

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
			// allow everyone to access this symbol
			accessor_public = 1,
			// only access to this symbol from the current module
			accessor_module,
			// only allow access from the same package
			accessor_private
		};

		/**
		 * \param view
		 * \param name the name of this symbol
		 */
		explicit node_symbol(const source_code_view& view)
				: node_symbol(view, accessor_public)
		{
		}

		/**
		 * \param view
		 * \param name the name of this symbol
		 * \param accessor whom are allowed to access this symbol
		 */
		node_symbol(const source_code_view& view, accessor accessor)
				: node(view), _accessor(accessor)
		{
		}


		/**
		 * \return a unique id for this symbol
		 */
		[[nodiscard]] virtual string get_id() const = 0;

		/**
		 * \return whom are allowed to access this symbol
		 */
		[[nodiscard]] accessor get_accessor() const
		{
			return _accessor;
		}

		/**
		 * \return true if the current node is allowed to access this symbol
		 */
		bool is_allowed(node* n) const;

		/**
		 * \brief helper function for testing for collisions
		 * \param t send in this from the inherited class
		 *
		 * note that you have to implement the method "bool compare_with_symbol(const T*) const" if you want to use
		 * this method
		 */
		template<class T>
		void test_collision(const T* t) const
		{
			node_symbol_collision_visitor v(t);
			const auto parent = get_parent_of_type<node_symbol>();
			parent->query(&v, query_flag_children | query_flag_downwards);
		}

#pragma region node

		void write_json_properties(json& j) override;

#pragma endregion

	private:
		accessor _accessor;
	};
}
