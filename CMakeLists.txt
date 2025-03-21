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
# 参数:
# <name> - 可执行程序的名称
# 头文件不需要指定，add_program 会自动添加 include 目录
# 例如:
# add_program(hello test/hello.cpp)
#
# add_program_options 用于添加可执行程序的依赖项、库和头文件路径
# 参数:
# DEPENDENCIES - 依赖项，可以是库、CMake 模块等
# LIBRARIES - 需要链接的库
# INCLUDES - 头文件路径
# 例如:
# add_program_options(
# DEPENDENCIES  # 依赖项
# LIBRARIES     # 需要链接的库
# INCLUDES      # 头文件路径
# )
# -----------------------------------------------------------------------------------------------------------------------
add_program_options(
    DEPENDENCIES

    LIBRARIES

    INCLUDES
    include
)

# -----------------------------------------------------------------------------------------------------------------------
add_program(hello test/hello.cpp)
add_program(date study/oj-1/date.cpp)
add_program(mylist study/oj-1/mylist.cpp)
add_program(a study/hcy/a.cpp)
add_program(b study/hcy/b.cpp)
add_program(c study/hcy/c.cpp)