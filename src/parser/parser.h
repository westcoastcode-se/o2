//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include "node_package.h"
#include "functions/node_func.h"
#include "functions/node_func_method.h"
#include "functions/node_func_body.h"
#include "variables/node_var_this.h"
#include "node_import.h"
#include "node_ref.h"
#include "types/node_type_accessor.h"
#include "types/node_type_array.h"
#include "struct/node_type_struct.h"
#include "struct/node_type_struct_field.h"
#include "operations/node_op_constant.h"
#include "node_scope.h"
#include "operations/node_op_unaryop.h"
#include "operations/node_op_binop.h"
#include "operations/node_op_return.h"
#include "syntax_tree.h"
#include "source_code.h"
#include "module/module.h"
#include "module/node_module.h"
#include "parser_state.h"

namespace o2
{
	/**
	 * \brief parse the supplied module and put the result into the syntax tree
	 * \param st the syntax tree
	 * \param m the module
	 * \param path path to where the main binary files are located
	 * \param imports a list of all imports that the source codes in the module want to load
	 * \return the package to be added to the syntax tree
	 */
	extern node_package* parse_module_file(syntax_tree* st, const module* m, source_code src,
			parser_state* state);

	/**
	 * \brief parse the supplied module and put the result into the syntax tree
	 * \param st the syntax tree
	 * \param m the module
	 * \param path path to where the main binary files are located
	 * \param imports a list of all imports that the source codes in the module want to load
	 * \return the package to be added to the syntax tree
	 */
	extern node_package* parse_module_path(syntax_tree* st, const module* m, string_view path,
			parser_state* state);

	/**
	 * \brief parse the supplied list of sources
	 * \param sources
	 * \param package_name
	 * \param state
	 * \return the package node
	 */
	extern node_package* parse_module_import(array_view<source_code*> sources, string_view package_name,
			parser_state* state);

	/**
	 * \brief parse the supplied module and put the result into the syntax tree
	 * \param st the syntax tree
	 * \param m the module
	 * \param path path to where the main binary files are located
	 * \param imports a list of all imports that the source codes in the module want to load
	 * \return the package to be added to the syntax tree
	 */
	extern node_package* parse_module_import(syntax_tree* st, const module* m, string_view path,
			parser_state* state);

	/**
	 * \brief optimize the syntax tree 
	 * \param st 
	 * \param level 
	 */
	extern void optimize(syntax_tree* st, int level);
}
