cmake_minimum_required(VERSION 2.8)
project (DS_UTIL)

find_package(OpenCV REQUIRED)
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -ftest-coverage -fprofile-arcs")
include_directories(${INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(2bin img2bin.cpp)
add_executable(extrc faceCorpper.cpp)
add_executable(genl genlist.cpp)
add_executable(sepl sepl.cpp)

target_link_libraries(2bin ${OpenCV_LIBS} )
target_link_libraries(extrc ${OpenCV_LIBS} )
