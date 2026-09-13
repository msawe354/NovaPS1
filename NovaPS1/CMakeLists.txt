cmake_minimum_required(VERSION 3.20)
project(Nova-PS1 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE NOVA_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Core/*.cpp
)

add_executable(nova-ps1
    ${NOVA_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_include_directories(nova-ps1 PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(nova-ps1 PRIVATE /W4 /WX-)
else()
    target_compile_options(nova-ps1 PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(nova-ps1 PRIVATE -O2)
else()
    target_compile_options(nova-ps1 PRIVATE -g -O0)
endif()