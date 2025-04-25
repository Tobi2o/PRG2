cmake_minimum_required(VERSION 3.24)
project(Labo3 C)

set(CMAKE_C_STANDARD 17 -std=c17 -Wall -Wconversion -Wextra -Wsign-conversion -Wpedantic)

add_executable(Labo3 main.c bateau.c port.c liste_taxes.c)
