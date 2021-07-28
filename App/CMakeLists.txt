cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME RanokCoreApp)
project(${PROJECT_NAME} LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_BINARY_DIR}/../RanokCore/libRanokCore.dll ${CMAKE_CURRENT_BINARY_DIR}/libRanokCore.dll)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_CURRENT_BINARY_DIR}/../RanokCore/libRanokCore.dll.a ${CMAKE_CURRENT_BINARY_DIR}/libRanokCore.dll.a)
endif()


file(GLOB_RECURSE EXAMPLES ../Examples/*.txt)
add_executable(${PROJECT_NAME} main.cpp ${EXAMPLES})


target_link_libraries (${PROJECT_NAME} LINK_PUBLIC RanokCore)
target_include_directories(${PROJECT_NAME} PUBLIC RanokCore)
