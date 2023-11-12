//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node.h"

namespace o2
{
	/**
	 * \brief a node that refers to another node in the syntax tree
	 *
	 * resolving the actual node is done during the resolution phase
	 */
	class node_ref
			: public node
	{
	public:
		enum query_types
		{
			package = 1 << 0,
			type = 1 << 1,
			func = 1 << 2,
			method = 1 << 3,
			arg = 1 << 4,
			local = 1 << 5,
			global = 1 << 6,
			primitive = 1 << 7,
			const_ = 1 << 8
		};

		static constexpr int only_types = type | primitive;

		/**
		 * \brief types that limits to only locations where we can read and write data to
		 */
		static constexpr int only_variables = arg | local | global;

		/**
		 * \brief types that limit to only locations where one can find a function to call
		 */
		static constexpr int only_callable = arg | local | global | func | method;

		/**
		 * \brief Query types that allow for a chain of references until we get to a function to call
		 */
		static constexpr int only_callable_chain_types = package | type | func;

		/**
		 * \brief Query types that allow for a chain of references until we get to a type
		 */
		static constexpr int only_type_chain_types = package | type;

		node_ref(const source_code_view& view, int types, int flags, string_view text)
				: node(view), _query_types(types), _query_flags(flags), _text(text)
		{
		}

		node_ref(const source_code_view& view, node* result);
		
		/**
		 * \return types that this reference is looking for during the resolution phase
		 */
		int get_query_types() const
		{
			return _query_types;
		}

		/**
		 * \param qt the query types we want to set
		 */
		void set_query_types(int qt)
		{
            _query_types = qt;
		}

		/**
		 * \return the text we are querying
		 */
		string_view get_query_text() const
		{
			return _text;
		}

		/**
		 * \return all results
		 */
		array_view<node*> get_result() const
		{
			return _results;
		}

#pragma region node

		void debug(std::basic_ostream<char>& stream, int indent) const final;

		bool resolve(const recursion_detector* rd) final;

#pragma endregion

		bool resolve_from_parent(node* parent);

	private:
		int _query_types;
        int _query_flags;
		string_view _text;
		vector<node*> _results;
	};
}