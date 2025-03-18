cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(fix_msvc)
include(cpp_novice_fetch_project_options)

project(cpp_novice LANGUAGES C CXX)

include(check_latest_cppstd)
include(cpp_novice_custom_project_options)
include(add_program)

# -----------------------------------------------------------------------------------------------------------------------
# add_program 用于添加一个可执行程序
# 语法: add_program(<name> <source> [INCLUDES <include_dirs>])
# 参数:
# <name> - 可执行程序的名称
# <source> - 源文件路径
# [INCLUDES <include_dirs>] - 可选参数，指定头文件目录
# 示例:
# add_program(hello hello.cpp INCLUDES include)
add_program_options(
    DEPENDENCIES

    LIBRARIES

    INCLUDES
    include
)

# -----------------------------------------------------------------------------------------------------------------------
add_program(hello hello.cpp INCLUDES hello.h)