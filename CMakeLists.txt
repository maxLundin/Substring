cmake_minimum_required(VERSION 3.13)
project(Substring)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic -O3 ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/scanners)
include_directories(${CMAKE_SOURCE_DIR}/scanners/trigram)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

add_subdirectory(scanners)


add_executable(Substring
        main.cpp
        mainwindow.cpp
        mainwindow.h
        text_input.cpp
        text_input.h
        debug_cout.h)


target_link_libraries(Substring lib_trigram)
target_link_libraries(Substring scanners_lib)

qt5_use_modules(Substring Widgets Core)