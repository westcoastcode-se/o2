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
		explicit parser_state(syntax_tree* st)
				: _syntax_tree(st), _source_code()
		{
		}

		/**
		 * \return the source code that's being parsed
		 */
		[[nodiscard]] const source_code* get_source_code() const
		{
			return _source_code;
		}

		void set_source_code(const source_code* source)
		{
			_source_code = source;
		}

		/**
		 * \return all imports collected during the parse phase
		 */
		[[nodiscard]] vector<node_import*> get_imports()
		{
			return std::move(_imports);
		}

		/**
		 * \brief add an import
		 * \param i
		 */
		void add_import(node_import* i);

		/**
		 * \brief
		 * \param type
		 * \return the node that represents the supplied primitive type
		 */
		[[nodiscard]] node_type_primitive* get_primitive_type(primitive_type type) const;

		/**
		 * \brief try to find a primitive with the supplied name
		 * \param name the primitive type name
		 * \return the node that represents the supplied primitive type
		 */
		[[nodiscard]] node_type_primitive* find_primitive_type(string_view name) const;

		/**
		 * \return the primitive that represents a byte
		 */
		[[nodiscard]] node_type_primitive* get_primitive_byte() const;

	private:
		syntax_tree* const _syntax_tree;
		const source_code* _source_code;
		vector<node_import*> _imports;
	};
}