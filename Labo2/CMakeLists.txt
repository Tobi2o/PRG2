cmake_minimum_required(VERSION 3.24)
project(Labo_2 C)
add_compile_options(--std=c17 -Wall -Wconversion -Wextra -Wsign-conversion -Wpedantic )
set(CMAKE_C_STANDARD 17 )

add_executable(Labo_2 main.c listes_dynamiques.c)
