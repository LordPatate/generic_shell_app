cmake_minimum_required(VERSION 3.23)

project(ShellApp)

add_subdirectory(sources/res_or_err)
add_subdirectory(sources/array_builder)
add_subdirectory(sources/readline)
add_subdirectory(sources/cmdline_parser)
add_subdirectory(sources/prefix_trees)
add_subdirectory(sources/shell_app)
add_subdirectory(examples/default_app)
add_subdirectory(examples/extended_state)
