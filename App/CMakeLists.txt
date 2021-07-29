cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME RanokCoreApp)
project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE EXAMPLES ../Examples/*.txt)
add_executable(${PROJECT_NAME} main.cpp ${EXAMPLES})


target_link_libraries (${PROJECT_NAME} LINK_PUBLIC RanokCore)
target_include_directories(${PROJECT_NAME} PUBLIC RanokCore)
