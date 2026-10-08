cmake_minimum_required(VERSION 3.20)
project(RayC DESCRIPTION "A compiler for a custom programming language named Ray." LANGUAGES CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are disallowed. Please run: cmake -B build")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_R "Build for release" OFF)
option(BUILD_D "Build for debug" ON)
option(ENABLE_ASAN "Enable AddressSanitizer for memory safety" OFF)

add_library(c_warnings INTERFACE)

if(BUILD_D)
    # Enable compiler warnings
    if(MSVC)
        target_compile_options(c_warnings INTERFACE /W4 /permissive-)
    else()
        target_compile_options(c_warnings INTERFACE 
            -Wall 
            -Wextra 
            -Wpedantic 
            -Wshadow
            -Wconversion
            -Wnon-virtual-dtor
            -Wold-style-cast 
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
        )
    endif()
endif()

if(BUILD_R)
    if(MSVC)
        target_compile_options()
    endif()
endif()

if(ENABLE_ASAN AND NOT MSVC)
    target_compile_options(c_warnings INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(c_warnings INTERFACE -fsanitize=address)
endif()


add_library(rayc_core STATIC 
    src/assgen.cpp
    src/parser.cpp
    src/lexer.cpp
)

target_include_directories(rayc_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(rayc_core PRIVATE c_warnings)

# Executable target
add_executable(rayc src/cli-main.cpp)

# Link Core
target_link_libraries(rayc PRIVATE rayc_core c_warnings)