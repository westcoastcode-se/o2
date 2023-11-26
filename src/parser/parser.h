//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "package/node_package.h"
#include "functions/node_func.h"
#include "functions/node_func_method.h"
#include "functions/node_func_body.h"
#include "variables/node_var_this.h"
#include "node_import.h"
#include "node_ref.h"
#include "types/node_type_pointer_of.h"
#include "types/node_type_reference_of.h"
#include "types/node_type_array.h"
#include "struct/node_type_struct.h"
#include "struct/node_type_struct_field.h"
#include "operations/node_op_constant.h"
#include "node_scope.h"
#include "operations/node_op_unaryop.h"
#include "operations/node_op_binop.h"
#include "operations/node_op_return.h"
#include "operations/node_op_callfunc.h"
#include "syntax_tree.h"
#include "source_code.h"
#include "module/module.h"
#include "parser_state.h"

namespace o2
{
	/**
	 * \brief parse the supplied list of sources and return a package that represents
	 *        that source code
	 * \param sources source code that's part of the new package that we are about to create
	 * \param package_name the name of the package
	 * \param state a state in which
	 * \return the package node
	 * \remark it's the responsibility of the caller to add the package to the module node and to, potentially,
	 *         parse includes collected during the parsing of the source code
	 */
	extern node_package* parse_package_sources(array_view<source_code*> sources, string_view package_name,
			OUT parser_state* state);

	/**
	 * \brief optimize the syntax tree 
	 * \param st 
	 * \param level 
	 */
	extern void optimize(syntax_tree* st, int level);
}
