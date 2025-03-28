# - Wraps cmake to simplify the work of adding a new program
#
# The below function will create a new executable target, then novices could build it, run it or debug it as normal.
#
# add_program(<program_name> [source_files1...])
#
# The below function will add dependencies, libraries and includes to the global config, thus they're usable by all
# programs added by `add_program`.
#
# add_program_options(
# [DEPENDENCIES [dependency1...]]
# [LIBRARIES [library1...]]
# [INCLUDES [include1...]]
# )
#
include_guard()

if(NOT TARGET code_options)
    add_library(code_options INTERFACE)
endif()

function(add_program_options)
    set(options)
    set(one_value_args)
    set(multi_value_args DEPENDENCIES LIBRARIES INCLUDES)

    cmake_parse_arguments(args "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    foreach(dependency IN LISTS args_DEPENDENCIES)
        find_package(${dependency} CONFIG REQUIRED)
    endforeach()

    target_link_system_libraries(code_options INTERFACE ${args_LIBRARIES})
    target_include_directories(code_options INTERFACE ${args_INCLUDES})
endfunction()

function(add_program name)
    set(options)
    set(one_value_args)
    set(multi_value_args INCLUDES)

    cmake_parse_arguments(args "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

    # 处理源文件路径，将相对路径转换为绝对路径
    set(sources)

    foreach(src ${args_UNPARSED_ARGUMENTS})
        if(NOT IS_ABSOLUTE ${src})
            get_filename_component(abs_src "${src}" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
            list(APPEND sources ${abs_src})
        else()
            list(APPEND sources ${src})
        endif()
    endforeach()

    # 处理头文件路径，将相对路径转换为绝对路径
    set(includes)

    foreach(inc ${args_INCLUDES})
        if(NOT IS_ABSOLUTE ${inc})
            get_filename_component(abs_inc "${inc}" ABSOLUTE BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
            list(APPEND includes ${abs_inc})
        else()
            list(APPEND includes ${inc})
        endif()
    endforeach()

    add_executable(${name})
    target_sources(${name}
        PRIVATE
        ${sources}
    )
    target_include_directories(${name}
        PRIVATE
        ${includes}
    )
    target_link_libraries(${name}
        PRIVATE
        cpp_novice_project_options
        cpp_novice_project_warnings
    )
    target_link_system_libraries(${name}
        PRIVATE
        code_options
    )
endfunction()