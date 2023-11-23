//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"
#include "../variables/node_var.h"

namespace o2
{
	/**
	 * \brief node that represents where all the arguments can be found
	 */
	class node_func_parameters
			: public node
	{
	public:
		explicit node_func_parameters(const source_code_view& view);

		/**
		 * \return the number of arguments
		 */
		[[nodiscard]] int num_arguments() const;

		/**
		 * \return all arguments used by this function
		 */
		[[nodiscard]] array_view<node_var*> get_parameters() const
		{
			return _arguments;
		}

#pragma region node

		void debug(debug_ostream& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		vector<node_var*> _arguments;
	};
}