cmake_minimum_required(VERSION 3.17)
project(Magma)

include(cmake/Ide.cmake)
include(cmake/Utils.cmake)

#---------------------------------------------------------------------------------------
# Set build configuration
#---------------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose Release or Debug" FORCE)
endif()

project(Magma VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Build Magma: 0.0.1")

#---------------------------------------------------------------------------------------
# Compiler configuration
#---------------------------------------------------------------------------------------
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

#---------------------------------------------------------------------------------------
# Packages and dependencies
#---------------------------------------------------------------------------------------

add_subdirectory(submodules/glfw)
add_subdirectory(submodules/spdlog)
add_subdirectory(submodules/glm)

find_package(Vulkan REQUIRED)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(MAGMA_INCLUDE_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src)


add_library(${PROJECT_NAME}.Core SHARED ${MAGMA_CORE_HEADERS} src/Magma.Core/Devices/PhysicalDevice.cpp src/Magma.Core/Devices/QueueFamilies.h src/Magma.Core/Devices/QueueFamilies.cpp src/Magma.Core/Devices/LogicalDevice.h src/Magma.Core/Devices/LogicalDevice.cpp src/Magma.Core/Devices/Surface.h src/Magma.Core/Devices/Surface.cpp src/Magma.Core/Graphics/Renderpass/Swapchain.h src/Magma.Core/Graphics/Renderpass/Swapchain.cpp src/Magma.Core/Graphics/Pipelines/Shader.h src/Magma.Core/Graphics/Pipelines/Shader.cpp src/Magma.Core/Filesystem/File.cpp src/Magma.Core/Filesystem/File.h src/Magma.Core/Graphics/Graphics.cpp src/Magma.Core/Graphics/Graphics.h src/Magma.Core/Engine/Module.h src/Magma.Core/Graphics/Pipelines/GraphicsPipeline.cpp src/Magma.Core/Graphics/Pipelines/GraphicsPipeline.h src/Magma.Core/Graphics/Pipelines/ComputePipeline.cpp src/Magma.Core/Graphics/Pipelines/ComputePipeline.h src/Magma.Core/Graphics/Renderpass/Renderpass.cpp src/Magma.Core/Graphics/Renderpass/Renderpass.h src/Magma.Core/Graphics/Renderpass/Framebuffer.cpp src/Magma.Core/Graphics/Renderpass/Framebuffer.h src/Magma.Core/Graphics/Commands/CommandPool.cpp src/Magma.Core/Graphics/Commands/CommandPool.h src/Magma.Core/Graphics/Commands/CommandBuffer.cpp src/Magma.Core/Graphics/Commands/CommandBuffer.h src/Magma.Core/Graphics/Renderer2D.h src/Magma.Core/Graphics/Renderer2D.cpp src/Magma.Core/Graphics/Buffers/Buffer.cpp src/Magma.Core/Graphics/Buffers/Buffer.h src/Magma.Core/Graphics/Buffers/UniformBuffer.cpp src/Magma.Core/Graphics/Buffers/UniformBuffer.h)

target_sources(${PROJECT_NAME}.Core PRIVATE
        src/Magma.Core/Window.cpp src/Magma.Core/Engine/Log.cpp
        src/Magma.Core/Devices/Instance.cpp src/Magma.Core/Debug/ValidationLayers.cpp src/Magma.Core/Devices/Ext.cpp
        src/Magma.Core/Engine/Engine.cpp
        )

target_compile_definitions(${PROJECT_NAME}.Core PRIVATE _Magma_Core)

target_include_directories(${PROJECT_NAME}.Core PUBLIC ${Vulkan_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/submodules/spdlog/include)
target_include_directories(${PROJECT_NAME}.Core PUBLIC
        "$<BUILD_INTERFACE:${MAGMA_INCLUDE_HEADERS}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_link_libraries(${PROJECT_NAME}.Core PUBLIC glfw Vulkan::Vulkan glm::glm spdlog::spdlog)

set_target_properties(${PROJECT_NAME}.Core PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME}.Client ${MAGMA_CLIENT_HEADERS} src/Magma.Core/Filesystem/File.cpp src/Magma.Core/Filesystem/File.h)

target_sources(${PROJECT_NAME}.Client PRIVATE
        src/Magma.Client/EntryPoint.cpp
        )

target_compile_definitions(${PROJECT_NAME}.Client PRIVATE _Magma_Core)
target_link_libraries(${PROJECT_NAME}.Client PRIVATE ${PROJECT_NAME}.Core)

set_target_properties(${PROJECT_NAME}.Client PROPERTIES LINKER_LANGUAGE CXX)

if(WIN32)
    # copy the .dll file to the same folder as the executable
    add_custom_command(
            TARGET ${PROJECT_NAME}.Client
            POST_BUILD
            COMMAND
            ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/scripts
            $<TARGET_FILE_DIR:${PROJECT_NAME}.Client>
            VERBATIM)
endif()