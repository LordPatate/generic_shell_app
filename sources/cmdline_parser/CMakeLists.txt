cmake_minimum_required(VERSION 3.23)
add_library(CmdLineParser)
target_sources(CmdLineParser
    PRIVATE
        cmdline_parser.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            cmdline_parser.h
)
target_link_libraries(CmdLineParser
    PRIVATE
        ResOrErr
        ArrayBuilder
)
