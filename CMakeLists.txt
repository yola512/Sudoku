cmake_minimum_required(VERSION 3.30)
project(Sudoku C)

set(CMAKE_C_STANDARD 23)

add_executable(Sudoku main.c utils.c sudoku.c)