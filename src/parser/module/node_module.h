//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"
#include "module.h"

namespace o2
{
	/**
	 * \brief node that represents a module
	 *
	 * all modules are initially created by the modules parser phase. Packages are then put into their corresponding
	 * module during the source code parse phase
	 *
	 * modules can have child modules if they are marked as privately required. For example, the structure
	 * might look something like this:
	 *
	 * root
	 * |- module github.com/my_company/framework
	 *    |- package /
	 *    |- package /services
	 * |- module github.com/another_company/lib
	 *    |- module scope=private abcde.io/lib
	 *       |- package /
	 *    |- package /
	 *    |- package /parser
	 */
	class node_module final
			: public node
	{
	public:
		/**
		 * move the ownership of the module into this node
		 *
		 * \param view
		 * \param m
		 */
		node_module(const source_code_view& view, module* m)
				: node(view), _module(m)
		{
		}

		~node_module() final;

		const module* get_module() const
		{
			return _module;
		}

		string_view get_name() const
		{
			return _module->get_name();
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

		void query(query_node_visitor* visitor, int flags) final;

#pragma endregion

	private:
		module* const _module;
	};
}
