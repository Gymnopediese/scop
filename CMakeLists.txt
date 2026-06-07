cmake_minimum_required(VERSION 3.20)
project(VulkanGLFWProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Homebrew prefix (works for Apple Silicon + Intel)
execute_process(
    COMMAND brew --prefix
    OUTPUT_VARIABLE HOMEBREW_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# GLFW
execute_process(
    COMMAND brew --prefix glfw
    OUTPUT_VARIABLE GLFW_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Vulkan (MoltenVK via Vulkan SDK or Homebrew)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*/*.cpp
    src/*/*/*.cpp
)

add_executable(scop
    src/vulkan/SwapChain/SwapChain.cpp
    ${SOURCES}
)

# Includes
target_include_directories(scop PRIVATE
    ${GLFW_PREFIX}/include
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Libraries
target_link_directories(scop PRIVATE
    ${GLFW_PREFIX}/lib
)

target_link_libraries(scop PRIVATE
    glfw
    Vulkan::Vulkan
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework QuartzCore"
)

execute_process(
    COMMAND brew --prefix glm
    OUTPUT_VARIABLE GLM_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(scop PRIVATE
    ${GLFW_PREFIX}/include
    ${GLM_PREFIX}/include
    ${Vulkan_INCLUDE_DIRS}
)

# macOS Vulkan portability (MoltenVK)
target_compile_definitions(scop PRIVATE VK_USE_PLATFORM_MACOS_MVK)