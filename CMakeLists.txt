cmake_minimum_required(VERSION 3.22)


project(LogSystem)
# set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)
set (CMAKE_BUILD_TYPE Debug)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB source_file ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(LoggerTest ${CMAKE_SOURCE_DIR}/LoggerTest.cpp ${source_file})





