cmake_minimum_required(VERSION 3.16)
project(postfix)

set(CMAKE_CXX_STANDARD 17)

add_executable(postfix main.c)

target_sources(postfix PRIVATE
    Nodes.c
    Stack.c
    String.c
)


target_sources(postfix PRIVATE
    QOL.c
)
