project(DaintyWood)

include(./cmake/FindVulkan.cmake)
include(./cmake/FindGLFW.cmake)
include(./cmake/FindSpdlog.cmake)
include(./cmake/FindTomlpp.cmake)
include(./cmake/FindAssimp.cmake)

add_library(DWE
    STATIC
    ./lib/Core/Application.cc
    ./lib/Core/Logger.cc
    ./lib/Core/Window.cc
    ./lib/Core/Renderer.cc
    ./lib/Core/ResourceManager.cc
    ./lib/Events/Event.cc
    ./lib/Events/KeyEvents.cc
    ./lib/Events/MouseEvents.cc
    ./lib/Events/WindowEvents.cc
    ./lib/Vulkan/ShaderSettings.cc
    ./lib/Vulkan/VulkanInstance.cc
    ./lib/Vulkan/VulkanMesh.cc
    ./lib/Vulkan/VulkanRenderEntity.cc
    ./lib/Vulkan/VulkanShader.cc
    ./lib/Vulkan/VulkanTexture.cc
    ./lib/Vulkan/VulkanUtils.cc
)
target_include_directories(DWE
    PRIVATE
    ./include
)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(DWE PRIVATE -D DWE_DEBUG)
endif ()

set_target_properties(DWE
    PROPERTIES
    PREFIX ""
    OUTPUT_NAME "DWElib"
    SUFFIX ".a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include")
target_precompile_headers(DWE
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/pch.hh)

target_link_libraries(DWE
    PRIVATE
    vulkan
    glfw
    spdlog::spdlog # remember to link spdlog as "spdlog::spdlog" or there will be link error
    tomlplusplus
    assimp
)
