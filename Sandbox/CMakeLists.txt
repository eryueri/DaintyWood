project(DW-Sandbox)

add_executable(sandbox
    ./src/main.cc)
add_dependencies(sandbox
    DWE)

target_link_libraries(sandbox
    PRIVATE
    DWE)
