//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	template<class T>
	class node_op_optimizer
			: public node_optimizer
	{
	public:
		bool accept(const node* n) override
		{
			return dynamic_cast<const T*>(n) != nullptr;
		}
	};

	/**
	 * \brief base class for operations
	 */
	class node_op
			: public node
	{
	public:
		enum modifier
		{
			modifier_none = 0,

			// Indicates that this operation is constant
			modifier_const = 1 << 0,

			modifier_next_bit = 1 << 1
		};

		node_op(const source_code_view& view)
				: node(view), _op_modifiers(modifier_none)
		{
		}

		node_op(const source_code_view& view, int modifiers)
				: node(view), _op_modifiers(modifiers)
		{
		}

		/**
		 * \return modifiers applied to this node
		 */
		int get_op_modifiers() const
		{
			return _op_modifiers;
		}

		/**
		 * \param modifier
		 * \return
		 */
		bool has_op_modifier(int modifier) const
		{
			return bit_isset(_op_modifiers, modifier);
		}

	protected:
		int _op_modifiers;
	};
}