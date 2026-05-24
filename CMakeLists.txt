cmake_minimum_required(VERSION 3.10)
project(NovaStella VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
endif()

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/play.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC Includes/)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image fmt::fmt-header-only)

find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${MINIMP3_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()
