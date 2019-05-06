
project(Bingo CXX)
cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)

ENABLE_LANGUAGE(C)

set (CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} Bingo.cxx)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

