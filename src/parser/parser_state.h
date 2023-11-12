//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"
#include "types/node_type.h"
#include "types/node_type_primitive.h"
#include "primitive_value.h"

namespace o2
{
	class node_import;

	class syntax_tree;

	/**
	 * \brief the current parsing state
	 *
	 * you can reuse this object between threads, but you are not allowed to use the
	 * same instance in multiple threads at the same time
	 */
	class parser_state
	{
	public:
		parser_state(syntax_tree* st)
				: _syntax_tree(st), _source_code()
		{
		}

		inline syntax_tree* get_syntax_tree() const
		{
			return _syntax_tree;
		}

		inline const source_code* get_source_code() const
		{
			return _source_code;
		}

		inline void set_source_code(const source_code* source)
		{
			_source_code = source;
		}

		/**
		 * \return all nodes collected during the resolve phase
		 */
		inline vector<node_type*> get_resolve_size_nodes()
		{
			return std::move(_resolve_size);
		}

		/**
		 * \return all imports collected during the parse phase
		 */
		inline vector<string_view> get_imports()
		{
			return std::move(_imports);
		}

		/**
		 * \brief add the supplied node to the linked list
		 *
		 * \param n
		 */
		inline void add_resolve_size(node_type* n)
		{
			_resolve_size.add(n);
		}

		/**
		 * \brief add an import
		 * \param i
		 */
		void add_import(const node_import* i);

		/**
		 * \brief
		 * \param type
		 * \return the node that represents the supplied primitive type
		 */
		node_type_primitive* get_primitive_type(primitive_type type) const;

	private:
		syntax_tree* const _syntax_tree;
		const source_code* _source_code;
		vector<string_view> _imports;
		vector<node_type*> _resolve_size;
	};
}