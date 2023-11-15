//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"
#include "../variables/node_var.h"
#include "node_func_arguments.h"
#include "node_func_returns.h"

namespace o2
{
	class node_func_body;

	/**
	 * \brief base class for all types of functions, such as global functions, method functions and static functions
	 */
	class node_func
			: public node_symbol
	{
	public:
		enum modifier
		{
			modifier_const = 1 << 0,
			modifier_extern = 1 << 1,
		};

		/**
		 * \param view information of the source code
		 * \param name the name of the function
		 */
		node_func(const source_code_view& view, string_view name)
				: node_symbol(view), _name(name), _body(), _arguments(), _returns(), _modifiers()
		{
		}

		struct inner_function
		{
		};

		/**
		 * \brief specialized constructor for inner functions (function in function)
		 * \param view information of the source code
		 * \param name the name of the function
		 */
		node_func(const source_code_view& view, string_view name, inner_function)
				: node_symbol(view), _name(name), _body(), _arguments(), _returns(), _modifiers()
		{
			set_query_access_flags(query_access_modifier_no_siblings);
		}

		struct static_function
		{
		};

		/**
		 * \brief specialized constructor for static functions (function in static block inside a type)
		 * \param view information of the source code
		 * \param name the name of the function
		 */
		node_func(const source_code_view& view, string_view name, static_function)
				: node_symbol(view), _name(name), _body(), _arguments(), _returns(), _modifiers()
		{
			set_query_access_flags(query_access_modifier_no_siblings);
		}

		struct const_function
		{
		};

		/**
		 * \brief specialized constructor for const functions
		 * \param view information of the source code
		 * \param name the name of the function
		 */
		node_func(const source_code_view& view, string_view name, const_function)
				: node_symbol(view), _name(name), _body(), _arguments(), _returns(), _modifiers(modifier_const)
		{
			set_query_access_flags(query_access_modifier_no_siblings);
		}

		struct extern_function
		{
		};

		/**
		 * \brief specialized constructor for const functions
		 * \param view information of the source code
		 * \param name the name of the function
		 */
		node_func(const source_code_view& view, string_view name, extern_function)
				: node_symbol(view), _name(name), _body(), _arguments(), _returns(), _modifiers(modifier_extern)
		{
		}

		/**
		 * \return the name of the function
		 */
		string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return the body that this function executes
		 */
		node_func_body* get_body() const
		{
			return _body;
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

		/**
		 * \return true if this function is a constant function
		 */
		bool is_const() const
		{
			return bit_isset(_modifiers, modifier_const);
		}

		/**
		 * \return true if this function is an external function
		 */
		bool is_extern() const
		{
			return bit_isset(_modifiers, modifier_extern);
		}

#pragma region node_symbol

		void resolve_symbol_id() override;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const override;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

#pragma endregion

	private:
		string_view _name;
		node_func_body* _body;
		node_func_arguments* _arguments;
		node_func_returns* _returns;
		int _modifiers;
	};
}
