cmake_minimum_required(VERSION 3.12)
project("o2")
set(CMAKE_CXX_STANDARD 20)

# LLVM
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
llvm_map_components_to_libnames(llvm_libs support core irreader)

option(O2_MEMORY_TRACKING "should memory tracking be enabled or not" OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /EHsc /D_CRT_SECURE_NO_WARNINGS")
if (O2_MEMORY_TRACKING)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DO2_MEMORY_TRACKING")
endif ()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# Add source to this project's executable.
add_library(o2_parser "src/parser/parser.cpp"
        "src/parser/token.cpp"
        "src/parser/node.cpp"
        "src/parser/syntax_tree.cpp"
        "src/parser/error.cpp"
        "src/parser/package/node_package.cpp"
        "src/parser/functions/node_func.cpp"
        "src/parser/node_ref.cpp"
        "src/parser/node_import.cpp"
        "src/parser/variables/node_var.cpp"
        "src/parser/types/node_type_ref.cpp"
        "src/parser/types/node_type.cpp"
        "src/parser/types/node_type_array.cpp"
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
        "src/parser/functions/node_func_parameters.cpp"
        "src/parser/functions/node_func_returns.cpp"
        "src/parser/types/struct/node_type_struct.cpp"
        "src/parser/node_scope.cpp"
        "src/parser/types/struct/node_type_struct_field.cpp"
        "src/parser/node_symbol.cpp"
        "src/parser/operations/node_op_return.cpp"
        "src/parser/node_attribute.cpp"
        "src/parser/module/module.cpp"
        "src/parser/module/module_package_lookup.cpp"
        "src/parser/parser_state.cpp"
        "src/parser/recursion_detector.cpp"
        "src/parser/types/node_type_implicit.cpp"
        "src/parser/operations/node_op_assign.cpp"
        "src/parser/node_link.cpp"
        "src/parser/variables/node_var_const.cpp"
        "src/parser/functions/node_func_method.cpp"
        "src/parser/functions/node_func_body.cpp"
        "src/parser/variables/node_var_this.cpp"
        "src/parser/types/struct/node_type_struct_methods.cpp"
        "src/parser/types/struct/static/node_type_struct_static.cpp"
        "src/parser/types/struct/static/node_type_struct_static_funcs.cpp"
        "src/parser/types/struct/static/node_type_struct_static_vars.cpp"
        "src/parser/operations/node_op_callfunc.cpp"
        "src/parser/module/node_module.cpp"
        "src/parser/package/node_root.cpp"
        "src/parser/json/json_serializable.cpp"
        "src/parser/types/node_type_known_ref.cpp"
        "src/parser/types/node_type_pointer_of.cpp"
        "src/parser/types/node_type_reference_of.cpp"
        "src/parser/resolve_state.cpp"
        "src/parser/module/system_modules.cpp"
        "src/parser/types/struct/node_type_struct_inherits.cpp"
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
        "src/tests/attributes/attributes.cpp"
)
target_link_libraries(o2_tests o2_parser ${llvm_libs})

# CLI
add_executable(o2
        "src/cli/main.cpp"
        "src/cli/commands/build.cpp"
)
target_link_libraries(o2 o2_parser ${llvm_libs})