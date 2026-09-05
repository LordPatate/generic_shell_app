cmake_minimum_required(VERSION 3.23)
add_library(ResOrErr)
set_target_properties(ResOrErr
    PROPERTIES
        LINKER_LANGUAGE C
)
target_sources(ResOrErr
    PUBLIC
        FILE_SET HEADERS
        FILES
            res_or_err.h
)
