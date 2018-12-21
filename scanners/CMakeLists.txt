include_directories(${CMAKE_SOURCE_DIR})
include_directories(trigram)

add_subdirectory(trigram)

add_library(scanners_lib STATIC
        file_digger.cpp
        file_digger.h
        file_substring.cpp
        file_substring.h
        string_input.cpp
        string_input.h)

target_link_libraries(scanners_lib lib_trigram)