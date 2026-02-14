cmake_minimum_required(VERSION 3.10)
project(OpenGL-projects)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
# SETUP GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

# SETUP GLEW
add_library(glew STATIC vendor/glew/src/glew.c)
target_include_directories(glew PUBLIC vendor/glew/include)
target_compile_definitions(glew PUBLIC GLEW_STATIC)

# SETUP GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE vendor/glm)

# SETUP STB_IMAGE
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE vendor/stb_image)

# SETUP IMGUI 
add_subdirectory(vendor/imgui)

# PROJECTS
add_subdirectory(src/Showcase1)
add_subdirectory(src/Showcase2)
add_subdirectory(src/Showcase3)
