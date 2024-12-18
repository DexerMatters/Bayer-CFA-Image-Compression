cmake_minimum_required(VERSION 3.14.0)
project(bic VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

find_package(OpenCV REQUIRED)
find_library(LibRaw_LIBS raw)


include_directories(${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
add_executable(bic main.cpp ${SRC_FILES})
target_link_libraries(bic ${OpenCV_LIBS} ${LibRaw_LIBS})



