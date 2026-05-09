cmake_minimum_required(VERSION 3.10)
project(particle-physics)

set(SFML_DIR "C:/SFML/SFML-3.0.2/lib/cmake/SFML")
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System)

add_executable(particle-physics 
    src/main.cpp
    src/UI.cpp
    src/UI.h
)

target_link_libraries(particle-physics SFML::Graphics SFML::Window SFML::System)