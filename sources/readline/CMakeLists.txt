cmake_minimum_required(VERSION 3.23)
add_library(ReadLine)
target_sources(ReadLine
    PRIVATE
        readline.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            readline.h
)
target_link_libraries(ReadLine
    PRIVATE
        ResOrErr
        ArrayBuilder
)
