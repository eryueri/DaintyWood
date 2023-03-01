cmake_minimum_required(VERSION 3.17)

project(DWE-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(./DaintyWood)
add_subdirectory(./Sandbox)

add_custom_target(run ALL
    COMMAND ${DW-Sandbox_BINARY_DIR}/sandbox
    )
add_dependencies(run sandbox)

if (NOT 
       (EXISTS ${DaintyWood_SOURCE_DIR}/compile_commands.json 
        AND 
        EXISTS ${DW-Sandbox_SOURCE_DIR}/compile_commands.json)
    )
    execute_process(
        COMMAND ln -s ${CMAKE_BINARY_DIR}/compile_commands.json ${DaintyWood_SOURCE_DIR}/
        COMMAND ln -s ${CMAKE_BINARY_DIR}/compile_commands.json ${DW-Sandbox_SOURCE_DIR}/
        )
endif ()
