//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../../node.h"
#include "../node_type.h"

namespace o2
{
	/**
	 * \brief a container of inheritances
	 *
	 * All of this container's children are of the node_type_struct_inherit type
	 */
	class node_type_complex_inherits
			: public node
	{
	public:
		explicit node_type_complex_inherits(const source_code_view& view)
				: node(view)
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

		/**
		 * \brief search for a type that matches the supplied symbols
		 * \tparam str
		 * \param name
		 * \param s
		 * \return
		 */
		[[nodiscard]] node_type* find_inherited_type(string_view id) const;

		/**
		 * \brief check to see if we are inheriting from the supplied type
		 * \param type
		 * \return true, if the supplied type is inherited
		 * \remark please note that doing this type of search does not work
		 *         until child node references are resolved
		 */
		[[nodiscard]] bool inherits_from_type(node_type* type) const;

#pragma region node

		void resolve0(const recursion_detector* rd, resolve_state* state) final;

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion
	};

	/**
	 * \brief abstraction of an inheritance
	 *
	 * The only allowed children are:
	 * - node_type
	 */
	class node_type_complex_inherit
			: public node
	{
	public:
		explicit node_type_complex_inherit(const source_code_view& view)
				: node(view), _inherits_from()
		{
			set_query_access_flags(query_access_modifier_passthrough);
			add_phases_left(phase_resolve_size);
		}

		/**
		 * \return the size, in bytes, this inheritance results in
		 */
		[[nodiscard]] int get_size() const
		{
			return _inherits_from->get_size();
		}

		/**
		 * \return the type this struct inherits from
		 */
		[[nodiscard]] node_type* get_inherits_from() const
		{
			return _inherits_from;
		}

		/**
		 * \brief search for a type that matches the supplied symbols
		 * \tparam str
		 * \param name
		 * \param s
		 * \return
		 */
		[[nodiscard]] node_type* find_inherited_type(string_view id) const;

		/**
		 * \brief check to see if we are inheriting from the supplied type
		 * \param type
		 * \return true, if the supplied type is inherited
		 * \remark please note that doing this type of search does not work
		 *         until child node references are resolved
		 */
		[[nodiscard]] bool inherits_from_type(node_type* type) const;

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		void resolve0(const recursion_detector* rd, resolve_state* state) final;

		void on_process_phase(const recursion_detector* rd, resolve_state* state, int phase) final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		node_type* _inherits_from;
	};
}