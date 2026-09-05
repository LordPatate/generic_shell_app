cmake_minimum_required(VERSION 3.23)
add_executable(DefaultApp)
target_sources(DefaultApp
    PRIVATE
        main.c
)
target_link_libraries(DefaultApp
    PRIVATE
        ShellApp
)
