//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "package/node_root.h"
#include <llvm/IR/IRBuilder.h>

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
		explicit syntax_tree(llvm::LLVMContext& lc);

		/**
		 * \return the root package
		 */
		[[nodiscard]] node_root* get_root_package()
		{
			return &_root;
		}

		/**
		 * \return the root package
		 */
		[[nodiscard]] const node_root* get_root_package() const
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
		void debug(debug_ostream& stream) const;

		/**
		 * \brief try to optimize the syntax tree using the supplied optimizer
		 * \param optimizer 
		 */
		void optimize(node_optimizer* optimizer);

	private:
		node_root _root;

		llvm::LLVMContext& _context;
		llvm::IRBuilder<> _builder;
	};
}
