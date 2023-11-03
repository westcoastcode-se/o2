//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_package.h"

namespace o2
{
	/**
	 * \brief start of the entire syntax tree
	 *
	 * it contains a root package node that has basic primitives available
	 */
	class syntax_tree
	{
	public:
		syntax_tree();

		node_package* get_root_package()
		{
			return &_root;
		}

		const node_package* get_root_package() const
		{
			return &_root;
		}

		/**
		 * \brief print out debug information to stdout
		 */
		void debug() const;

		/**
		 * \brief output debug information about the entire syntax tree
		 * \param stream where to put the debug information into
		 */
		void debug(std::basic_ostream<char>& stream) const;

		/**
		 * \brief resolve all soft references, types etc
		 */
		void resolve();

		/**
		 * \brief try to optimize the syntax tree using the supplied optimizer
		 * \param optimizer 
		 */
		void optimize(node_optimizer* optimizer);

	private:
		node_package _root;
	};
}
