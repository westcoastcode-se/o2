//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"
#include "types/node_type.h"

namespace o2
{
	/**
	 * \brief an attribute that can be attached a keyword
	 *
	 * Keywords that can have attributes are:
	 * - type declarations
	 * - methods
	 * - functions
	 * - arguments
	 * - struct fields
	 * - constant value
	 * - variable
	 *
	 * An attribute must have the base class "stdlib/attribute"
	 *
	 * TODO add support for attribute arguments
	 */
	class node_attribute
			: public node
	{
	public:
		explicit node_attribute(const source_code_view& view);

		/**
		 * \return the attribute type, for example "link" if you specified "@link"
		 */
		[[nodiscard]] node_type* get_attribute_type() const
		{
			return _attribute_type;
		}

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		void resolve0(const recursion_detector* rd, resolve_state* state) final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		node_type* _attribute_type;
	};

	/**
	 * \brief a container for one or more attributes
	 */
	class node_attributes
			: public node
	{
	public:
		explicit node_attributes(const source_code_view& view);

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion
	};
}