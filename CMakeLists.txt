cmake_minimum_required(VERSION 3.16)
project(shadersPractice)

set(CMAKE_CXX_STANDARD 14)

add_executable(shadersPractice main.cpp lib/glad/src/glad.c)

# after downloading the glfw files, include the subdirectory for glfw using add_subdirectory
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)


# step 3 including a directory for glfw located in the project folder
include_directories(lib/glfw/include)
include_directories(lib/glad/include)
include_directories(lib/glm)


# step 4 target the project folder, and include glad and glfw.
#   Specify the GLFW_Library in brackets
target_link_libraries(shadersPractice glfw ${GLFW_LIBRARY})