//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "token.h"
#include "package/node_package.h"
#include "types/node_type.h"
#include "source_code.h"
#include "source_code_view.h"
#include "parser_state.h"

namespace o2
{
	/**
	 * \brief the scope for the current parsing position
	 */
	struct parser_scope
	{
		/**
		 * \brief current state
		 */
		parser_state* const state;

		/**
		 * \brief the token
		*/
		token* const t;

		/**
		 * \brief the closest parent node
		*/
		node* const closest;

		/**
		 * \brief the closest package
		*/
		node_package* const package;

		/**
		 * \brief the closest type node
		*/
		node_type* const type;

		/**
		 * \brief the closest function node
		*/
		node_func* const func;

		parser_scope(parser_state* state, token* t)
				: state(state), t(t), closest(),
				  package(), type(), func()
		{
		}

		parser_scope(const parser_scope* parent, node_package* pkg)
				: state(parent->state), t(parent->t), closest(pkg), package(pkg),
				  type(parent->type), func(parent->func)
		{
		}

		parser_scope(const parser_scope* parent, node_type* nt)
				: state(parent->state), t(parent->t), closest(nt),
				  package(parent->package), type(nt),
				  func(parent->func)
		{
		}

		parser_scope(const parser_scope* parent, node_func* nf)
				: state(parent->state), t(parent->t), closest(nf),
				  package(parent->package), type(parent->type),
				  func(nf)
		{
		}

		parser_scope(const parser_scope* parent, node* n)
				: state(parent->state), t(parent->t), closest(n),
				  package(parent->package), type(parent->type),
				  func(parent->func)
		{
		}

		inline source_code_view get_view() const
		{
			return source_code_view(state->get_source_code(), t);
		}
	};
}