//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"
#include "../variables/node_named_variable.h"
#include "node_func_arguments.h"
#include "node_func_returns.h"

namespace o2
{
	class node_func_body;

	/**
	 * \brief the function definition
	 */
	class node_func_def
			: public node_symbol
	{
	public:
		/**
		 * \brief modifiers that can be applied to a function
		 */
		enum modifiers
		{
			modifier_extern = 1 << 0,
			modifier_const = 1 << 1,
			modifier_macro = 1 << 2,
			modifier_inner_func = 1 << 3
		};

		node_func_def(const source_code_view& view, string_view name, int modifiers)
				: node_symbol(view), _name(name), _modifiers(modifiers), _body(), _arguments(), _returns()
		{
			if (bit_isset(modifiers, modifier_inner_func))
			{
				set_query_access_flags(query_access_modifier_no_siblings);
			}
		}

		/**
		 * \return the name of the function
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \brief set the body this definition executes when called
		 * \param body 
		 */
		void set_body(node_func_body* body)
		{
			_body = body;
		}

		/**
		 * \return the body that this function executes
		 */
		node_func_body* get_body() const
		{
			return _body;
		}

		bool is_const() const
		{
			return (_modifiers & modifier_const) == modifier_const;
		}

		bool is_macro() const
		{
			return (_modifiers & modifier_macro) == modifier_macro;
		}

		/**
		 * \return arguments necessary for this function
		 */
		node_func_arguments* get_arguments() const
		{
			return _arguments;
		}

		/**
		 * \return return value for this node
		 */
		node_func_returns* get_returns() const
		{
			return _returns;
		}

#pragma region node_symbol

		void resolve_symbol_id() final;

#pragma endregion

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

#pragma endregion

	private:
		string_view _name;
		const int _modifiers;
		node_func_body* _body;
		node_func_arguments* _arguments;
		node_func_returns* _returns;
	};

	/**
	 * \brief the function body
	 */
	class node_func_body
			: public node
	{
	public:
		node_func_body(const source_code_view& view)
				: node(view), _def()
		{
		}

		// \brief set the definition for this function body
		void set_def(node_func_def* def)
		{
			_def = def;
		}

		// \brief get the definition that refers to this body
		node_func_def* get_def() const
		{
			return _def;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion

	private:
		node_func_def* _def;
	};
}
