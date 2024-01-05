//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node_symbol.h"
#include "../variables/node_var.h"
#include "node_func_parameters.h"
#include "node_func_returns.h"
#include "../node_attribute.h"

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
				: node_symbol(view), _name(name), _attributes(), _body(), _parameters(), _returns(), _modifiers()
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
				: node_symbol(view), _name(name), _attributes(), _body(), _parameters(), _returns(), _modifiers()
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
				: node_symbol(view), _name(name), _attributes(), _body(), _parameters(), _returns(), _modifiers()
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
				: node_symbol(view), _name(name), _attributes(), _body(), _parameters(), _returns(), _modifiers(modifier_const)
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
				: node_symbol(view), _name(name), _attributes(), _body(), _parameters(), _returns(), _modifiers(modifier_extern)
		{
		}

		/**
		 * \return the name of the function
		 */
		[[nodiscard]] string_view get_name() const
		{
			return _name;
		}

		/**
		 * \return attributes associated with this function
		 */
		[[nodiscard]] node_attributes* get_attributes() const
		{
			return _attributes;
		}

		/**
		 * \return the body that this function executes
		 */
		[[nodiscard]] node_func_body* get_body() const
		{
			return _body;
		}

		/**
		 * \return arguments necessary for this function
		 */
		[[nodiscard]] node_func_parameters* get_parameters() const
		{
			return _parameters;
		}

		/**
		 * \return return value for this node
		 */
		[[nodiscard]] node_func_returns* get_returns() const
		{
			return _returns;
		}

		/**
		 * \return true if this function is a constant function
		 */
		[[nodiscard]] bool is_const() const
		{
			return bit_isset(_modifiers, modifier_const);
		}

		/**
		 * \return true if this function is an external function
		 */
		[[nodiscard]] bool is_extern() const
		{
			return bit_isset(_modifiers, modifier_extern);
		}

		/**
		 * \brief perform an early collision detection of this function
		 * \param rhs
		 * \return true if the supplied function is identical
		 */
		bool superficial_test_symbol_collision(const node_func* rhs) const;

		/**
		 * \brief perform eep collision detection of this function
		 * \param rhs
		 * \return
		 */
		void deep_test_symbol_collision(const node_func* rhs) const;

#pragma region node_symbol

		[[nodiscard]] string get_id() const override;

#pragma endregion

#pragma region node

		void debug(debug_ostream& stream, int indent) const override;

		void write_json_properties(json& j) override;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

		void on_parent_node(node* p) final;

		void resolve0(const recursion_detector* rd, resolve_state* state) override;

#pragma endregion

#pragma region json_serializable

		[[nodiscard]] string_view get_json_type() const override
		{
			return { "func" };
		}

#pragma endregion

	private:
		string_view _name;
		node_attributes* _attributes;
		node_func_body* _body;
		node_func_parameters* _parameters;
		node_func_returns* _returns;
		int _modifiers;
	};
}
