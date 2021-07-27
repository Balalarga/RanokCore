cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME RanokCore)
project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(OpenCL REQUIRED)


file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.h)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})


target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARY})
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${OpenCL_INCLUDE_DIRS})
