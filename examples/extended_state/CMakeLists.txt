cmake_minimum_required(VERSION 3.23)
add_executable(ExtendedState)
target_sources(ExtendedState
    PRIVATE
        main.c
)
target_link_libraries(ExtendedState
    PRIVATE
        ShellApp
)
