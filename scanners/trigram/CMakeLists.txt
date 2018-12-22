include_directories(${CMAKE_SOURCE_DIR})
add_library(lib_trigram STATIC
        trigram.cpp
        trigram.h
        trigram_counter.cpp
        trigram_counter.h
        trigram_tree.h
        trigram_tree.cpp
        trigram_tree.hpp)

qt5_use_modules(lib_trigram Widgets Core)