cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME RanokCore)
cmake_minimum_required(VERSION 3.14)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(OpenCL REQUIRED)

include_directories(src ${OpenCL_INCLUDE_DIRS})

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.h)
add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core ${OpenCL_LIBRARY})
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
