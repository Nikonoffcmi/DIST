cmake_minimum_required(VERSION 3.0)
project(http-server-client)

set(TARGET_NAME "http-server")
set(TARGET_NAME2 "http-client")

set(LIBHV_INCLUDE ${PROJECT_BINARY_DIR}/contrib/libhv/include/hv)

add_subdirectory(${PROJECT_SOURCE_DIR}/contrib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wempty-body -Wredundant-move -O2")

file(GLOB_RECURSE SOURCES_Server
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HTTPserver/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HTTPserver/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HTTPserver/*.h )

file(GLOB_RECURSE SOURCES_Client
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Utility/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HTTPclient/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HTTPclient/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HTTPclient/*.h )

set(SOURCE_DIR_HEADER "")
execute_process(
    COMMAND find src -type d
    OUTPUT_VARIABLE SOURCE_DIR_HEADER
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    string(REPLACE "\n" ";" SOURCE_DIR_HEADER ${SOURCE_DIR_HEADER})
    
include_directories(${SOURCE_DIR_HEADER})
include_directories(${LIBHV_INCLUDE})
    
add_executable(${TARGET_NAME} ${SOURCES_Server})
target_link_libraries(${TARGET_NAME} hv_static)

add_executable(${TARGET_NAME2} ${SOURCES_Client})
target_link_libraries(${TARGET_NAME2} hv_static)