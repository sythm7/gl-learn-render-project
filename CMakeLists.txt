cmake_minimum_required(VERSION 3.5.0)
project(tp_opengl VERSION 0.1)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(tp_opengl 
    ./src/main.cpp
    ./src/glad/glad.c
    ./src/glad/glad.h
)

target_link_libraries(tp_opengl 
    glfw
    OpenGL::GL
)