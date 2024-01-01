//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "collections/vector.h"

namespace o2
{
	class node;

	class node_package;

	/**
	 * \brief state for the currently running resolve
	 */
	class resolve_state
	{
	public:
		explicit resolve_state(node_package* package);

		/**
		 * \brief add the supplied node to be processed during the next resolve phase
		 * \param n the node
		 */
		void add_next_phase(node* n);

	private:
		friend class node_package;

		/**
		 * \return all nodes for this state
		 */
		vector<node*, 32> get_nodes()
		{
			return std::move(_nodes);
		}

	private:
		node_package* const _package;
		vector<node*, 32> _nodes;
	};
}