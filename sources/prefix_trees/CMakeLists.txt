cmake_minimum_required(VERSION 3.23)
add_library(PrefixTrees)
target_sources(PrefixTrees
    PRIVATE
        prefix_trees.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            prefix_trees.h
)
target_link_libraries(PrefixTrees
    PRIVATE
        ResOrErr
)
