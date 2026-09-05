cmake_minimum_required(VERSION 3.23)
add_library(ArrayBuilder)
target_sources(ArrayBuilder
    PRIVATE
        array_builder.c
        array_builder_internals.h
    PUBLIC
        FILE_SET HEADERS
        FILES
            array_builder.h
)
target_link_libraries(ArrayBuilder
    PRIVATE
        ResOrErr
)
