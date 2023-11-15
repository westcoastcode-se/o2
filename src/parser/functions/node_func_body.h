//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	class node_func;

	/**
	 * \brief the function body
	 */
	class node_func_body
			: public node
	{
	public:
		explicit node_func_body(const source_code_view& view)
				: node(view), _def()
		{
		}

		// \brief get the definition that refers to this body
		node_func* get_def() const
		{
			return _def;
		}

#pragma region node

		void on_parent_node(node* n) final;

		void on_removed_parent_node(node* n) final;

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		node_func* _def;
	};
}
