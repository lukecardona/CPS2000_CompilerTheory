cmake_minimum_required(VERSION 3.25)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled Parser.cpp lexer.h ErrorHandling.h parser.h XMLGeneration.cpp SemanticAnylsis.cpp Lexer.cpp CodeGen.cpp XMLGenerator.h "Sematic Anylsis.h")
