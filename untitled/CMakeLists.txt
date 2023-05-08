cmake_minimum_required(VERSION 3.25)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled Lexer.cpp Parser.cpp lexer.h ErrorHandling.h parser.h XMLGeneration.cpp visitor.h SemanticAnylsis.cpp CodeGeneration.cpp)
