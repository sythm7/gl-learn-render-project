cmake_minimum_required(VERSION 3.5.0)
project(tp_opengl VERSION 0.1)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

# Listes des fichiers sources et des exécutables
set(SOURCES_LIST
    "bezier_uniforme_parametrique.cpp"
    "bezier_surface.cpp"
)

set(EXECUTABLES_LIST
    "bezier_uniforme_parametrique"
    "bezier_surface"
)

if(WIN32)
    add_subdirectory(glfw/)
    add_subdirectory(glm/)
endif()

# Création des exécutables dans une boucle for
foreach(EXECUTABLE_NAME ${EXECUTABLES_LIST})
    add_executable(${EXECUTABLE_NAME} 
        "./src/${EXECUTABLE_NAME}.cpp"
        "./src/glad/glad.c"
        "./src/glad/glad.h"
    )

    if(WIN32)
        target_include_directories(${EXECUTABLE_NAME} PUBLIC win32)
        target_link_libraries(${EXECUTABLE_NAME}
            glm
        )
    endif()

    target_link_libraries(${EXECUTABLE_NAME}
        glfw
        OpenGL::GL
    )
endforeach()