#ifndef UNTITLED_ERRORHANDLING_H
#define UNTITLED_ERRORHANDLING_H

#include <iostream>
#include <exception>
#include <utility>

#define UNRECOGNIZED_CHAR_ERROR "The character you are trying to parse with the not part of the language: "

using namespace std;

class UnrecognizedCharacter : public exception {
public:
    explicit UnrecognizedCharacter(string  error) : errorMessage(std::move(error)) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

private:
    string errorMessage;
};

class FileError : public exception {
public:
    explicit FileError(string  error) : errorMessage(std::move(error)) {}

    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

private:
    string errorMessage;
};

#endif //UNTITLED_ERRORHANDLING_H
