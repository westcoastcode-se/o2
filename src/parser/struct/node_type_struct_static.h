//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "../node.h"

namespace o2
{
	class node_type_struct_fields;

	/**
	 * \brief static container for structs
	 */
	class node_type_struct_static
			: public node
	{
	public:
		/**
		 * \brief container for all variables inside the struct block
		 */
		class fields
				: public node
		{
		public:
			explicit fields(const source_code_view& view)
					: node(view)
			{
				set_query_access_flags(query_access_modifier_passthrough);
			}

#pragma region node

			void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion
		};

		/**
		 * \brief container for all functions inside the struct block
		 */
		class funcs
				: public node
		{
		public:
			explicit funcs(const source_code_view& view)
					: node(view)
			{
				set_query_access_flags(query_access_modifier_passthrough);
			}

#pragma region node

			void debug(std::basic_ostream<char>& stream, int indent) const final;

#pragma endregion
		};

		explicit node_type_struct_static(const source_code_view& view)
				: node(view), _fields(), _funcs()
		{
			set_query_access_flags(query_access_modifier_passthrough);
		}

		/**
		 * \return fields part of this struct
		 */
		fields* get_fields() const
		{
			return _fields;
		}

		/**
		 * \return functions part of this struct
		 */
		funcs* get_funcs() const
		{
			return _funcs;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		node* on_child_added(node* n) final;

		void on_child_removed(node* n) final;

#pragma endregion

	private:
		fields* _fields;
		funcs* _funcs;
	};
}