cmake_minimum_required(VERSION 3.5.0)
project(tp_opengl VERSION 0.1)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} 
    ./src/main.cpp
    ./src/glad/glad.c
    ./src/glad/glad.h
)

if(WIN32)
    add_subdirectory(glfw/)
    target_include_directories(${PROJECT_NAME} PUBLIC win32)
endif()

target_link_libraries(${PROJECT_NAME} 
    glfw
    OpenGL::GL
)