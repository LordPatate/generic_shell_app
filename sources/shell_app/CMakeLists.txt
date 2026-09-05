cmake_minimum_required(VERSION 3.23)

add_library(ShellApp)
target_link_libraries(ShellApp
    PUBLIC
        ResOrErr
        ArrayBuilder
        ReadLine
        CmdLineParser
        PrefixTrees
)
target_sources(ShellApp
    PRIVATE
        shell_app.c
        defaults.c
    PUBLIC
        FILE_SET HEADERS
        FILES
            shell_app.h
)
