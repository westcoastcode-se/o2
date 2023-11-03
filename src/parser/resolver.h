//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "syntax_tree.h"
#include "parser_state.h"

namespace o2
{
	/**
	 * \brief resolve references in the syntax tree
	 * \param st
	 */
	extern void resolve(syntax_tree* st, parser_state* ps);
}
