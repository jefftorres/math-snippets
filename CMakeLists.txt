cmake_minimum_required(VERSION 3.28)
project(math_snippets C)

set(CMAKE_C_STANDARD 11)

add_executable(monte_carlo monte_carlo.c)
add_executable(conway conway.c)