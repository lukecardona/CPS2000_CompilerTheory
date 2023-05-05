cmake_minimum_required(VERSION 3.25)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled Lexer.cpp Parser.cpp header.h)
