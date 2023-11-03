//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"
#include "../variables/node_named_variable.h"

namespace o2
{
	/**
	 * \brief node that represents where all the arguments can be found
	 */
	class node_func_arguments
			: public node
	{
	public:
		node_func_arguments(const source_code_view& view);

		/**
		 * \return the number of arguments
		 */
		int num_arguments() const;

		/**
		 * \brief
		 * \return all arguments used by this function
		 */
		array_view<node_named_variable*> get_arguments() const
		{
			return _arguments;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		vector<node_named_variable*> _arguments;
	};
}