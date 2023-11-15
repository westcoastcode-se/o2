//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"

namespace o2
{
	/**
	 * \brief a link between two nodes
	 */
	class node_link final
			: public node
	{
	public:
		explicit node_link(const source_code_view& view)
				: node(view), _link()
		{
		}

		~node_link() final;

		/**
		 * \return true if the link is broken
		 * \remark a broken link means that one of the nodes are removed, normally during the
		 *         optimization phase
		 */
		bool is_broken() const
		{
			return _link == nullptr;
		}

		/**
		 * \return the node that this link points to
		 */
		node* get_node() const;

		/**
		 * \return a new link associated with this one
		 */
		node_link* new_link();

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

#pragma endregion

	private:
		node_link* _link;
	};
}