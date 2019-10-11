cmake_minimum_required(VERSION 3.15)
project(OpenCVProject)

set(CMAKE_CXX_STANDARD 14)
set (OpenCV_DIR "C:\\opencv\\build\\install")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}\\cmake\\")

find_package(OpenCV REQUIRED)
include_directories(${INCLUDE_DIRS})
add_executable(OpenCVProject main.cpp)


target_link_libraries(OpenCVProject ${OpenCV_LIBS})