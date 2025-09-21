cmake_minimum_required(VERSION 3.29)
project(CSC271)
set(CMAKE_CXX_STANDARD 17)

# Add directories
include_directories(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/include
        ${CMAKE_SOURCE_DIR}/glad/include
        include)


#Add the directory containng the compiled GLFW library to the linker's search path
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64)

# Add GLAD source file
add_library(glad ${CMAKE_SOURCE_DIR}/glad/src/glad.c)

# Define the executable
add_executable(CSC271 main.cpp
        src/shaderprogram.cpp)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")

#find_package(OpenGL REQUIRED)

# Link GLFW, GLAD, and framework
target_link_libraries(CSC271
        glad
        glfw3
        opengl32
)