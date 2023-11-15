cmake_minimum_required(VERSION 3.12)
project("o2")
set(CMAKE_CXX_STANDARD 20)

option(O2_MEMORY_TRACKING "should memory tracking be enabled or not" OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /EHsc /D_CRT_SECURE_NO_WARNINGS")
if (O2_MEMORY_TRACKING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DO2_MEMORY_TRACKING")
endif ()

# Add source to this project's executable.
add_library(o2_parser "src/parser/parser.cpp"
        "src/parser/token.cpp"
        "src/parser/node.cpp"
        "src/parser/syntax_tree.cpp"
        "src/parser/error.cpp"
        "src/parser/node_package.cpp"
        "src/parser/functions/node_func.cpp"
        "src/parser/node_ref.cpp"
        "src/parser/node_import.cpp"
        "src/parser/variables/node_var.cpp"
        "src/parser/types/node_type_ref.cpp"
        "src/parser/types/node_type.cpp"
        "src/parser/types/node_type_array.cpp"
        "src/parser/types/node_type_accessor.cpp"
        "src/parser/operations/node_op_constant.cpp"
        "src/parser/operations/node_op_unaryop.cpp"
        "src/parser/operations/node_op_binop.cpp"
        "src/parser/primitive_value.cpp"
        "src/parser/memory.cpp"
        "src/parser/optimizations/primitive_value_not.cpp"
        "src/parser/optimizations/primitive_value_bit_not.cpp"
        "src/parser/optimizations/primitive_value_dec.cpp"
        "src/parser/optimizations/primitive_value_inc.cpp"
        "src/parser/optimizations/primitive_value_neg.cpp"
        "src/parser/optimizations/primitive_value_equals.cpp"
        "src/parser/optimizations/primitive_value_not_equals.cpp"
        "src/parser/optimizations/primitive_value_add.cpp"
        "src/parser/optimizations/primitive_value_compare.cpp"
        "src/parser/types/node_type_primitive.cpp"
        "src/parser/functions/node_func_arguments.cpp"
        "src/parser/functions/node_func_returns.cpp"
        "src/parser/struct/node_type_struct.cpp"
        "src/parser/node_scope.cpp"
        "src/parser/struct/node_type_struct_field.cpp"
        "src/parser/node_symbol.cpp"
        "src/parser/operations/node_op_return.cpp"
        "src/parser/node_attribute.cpp"
        "src/parser/module/module.cpp"
        "src/parser/module/module_package_lookup.cpp"
        "src/parser/parser_state.cpp"
        "src/parser/recursion_detector.cpp"
        "src/parser/resolver.cpp"
        "src/parser/types/node_type_implicit.cpp"
        "src/parser/operations/node_op_assign.cpp"
        "src/parser/node_link.cpp"
        "src/parser/variables/node_var_const.cpp"
        "src/parser/functions/node_func_method.cpp"
        "src/parser/functions/node_func_body.cpp"
        "src/parser/variables/node_var_this.cpp"
        "src/parser/struct/node_type_struct_methods.cpp"
        "src/parser/struct/static/node_type_struct_static.cpp"
        "src/parser/struct/static/node_type_struct_static_funcs.cpp"
        "src/parser/struct/static/node_type_struct_static_vars.cpp"
        "src/parser/operations/node_op_callfunc.cpp"
        "src/parser/module/node_module.cpp"
)

# Test
add_executable(o2_tests "src/tests/main.cpp"
        "src/tests/packages/packages.cpp"
        "src/tests/funcs/funcs.cpp"
        "src/tests/imports/imports.cpp"
        "src/tests/modules/modules.cpp"
        "src/tests/structs/structs.cpp"
        "src/tests/const/const.cpp"
        "src/tests/errors_resolve/errors_resolve.cpp"
        "src/tests/errors_parse/errors_parse.cpp"
)
target_link_libraries(o2_tests o2_parser)

# CLI
add_executable(o2
        "src/cli/main.cpp"
        "src/cli/commands/build.cpp"
)
target_link_libraries(o2 o2_parser)