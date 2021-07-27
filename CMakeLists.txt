cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME RanokCoreApp)
project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(./RanokCoreLib RanokCore)
add_subdirectory(./App App)
