#ifndef UNTITLED_LEXER_H
#define UNTITLED_LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <memory>
#include <stack>
#include <tuple>
#include <windows.h>

#define DELIMITERS "*/+-=!={}(),<>:; "
#define FINAL_STATE "1"
#define LETTER_TOKEN "<letter>"
#define DIGIT_TOKEN "<digit>"
#define MULTIPLY_VALUE '*'
#define DIVISION_VALUE '/'
#define ADDITION_VALUE '+'
#define SUBTRACTION_VALUE '-'
#define LESS_THAN_VALUE '<'
#define GREATER_THAN_VALUE '>'
#define EQUALS_VALUE '='
#define NOT_VALUE '!'
#define OPEN_CURLY_BRACKET_VALUE '{'
#define CLOSE_CURLY_BRACKET_VALUE '}'
#define COMMA_TOKEN "<comma>"
#define HASH_VALUE '#'
#define UNDERSCORE_VALUE '_'
#define DOT_VALUE '.'
#define OPEN_BRACKET_VALUE '('
#define CLOSE_BRACKET_VALUE ')'
#define COLON_VALUE ':'
#define SEMICOLON_VALUE ';'
#define MULTIPLY_TOKEN "*"
#define DIVISION_TOKEN "/"
#define ADDITION_TOKEN "+"
#define SUBTRACTION_TOKEN "<->"
#define LESS_THAN_TOKEN "<"
#define GREATER_THAN_TOKEN ">"
#define EQUALS_TOKEN "<=>"
#define ARROW_TOKEN "->"
#define NOT_TOKEN "!"
#define OPEN_CURLY_BRACKET_TOKEN "<{>"
#define CLOSE_CURLY_BRACKET_TOKEN "<}>"
#define COMMA_TOKEN "<comma>"
#define HASH_TOKEN "#"
#define UNDERSCORE_TOKEN "_"
#define DOT_TOKEN "<.>"
#define OPEN_BRACKET_TOKEN "<(>"
#define CLOSE_BRACKET_TOKEN "<)>"
#define COLON_TOKEN "<:>"
#define SEMICOLON_TOKEN "<;>"
#define EMPTY_SPACE_TOKEN "<delim>"
#define INVALID_TOKEN "ERR"
#define START_STATE "1"
#define WORD "WORD"
#define TYPE "<Type>"
#define BOOLEAN_LITERAL "<BooleanLiteral>"
#define COLOR_LITERAL "<ColourLiteral>"
#define INTEGER_LITERAL "<IntegerLiteral>"
#define FLOAT_LITERAL "<FloatLiteral>"
#define SPECIAL_VAR "<SpecialVar>"
#define SPECIAL_WIDTH "__width"
#define SPECIAL_HEIGHT "__height"
#define SPECIAL_READ "__read"
#define SPECIAL_RANDI "__randi"
#define SPECIAL_PRINT "__print"
#define SPECIAL_DELAY "__delay"
#define SPECIAL_PIXEL "__pixel"
#define SPECIAL_PIXELR "__pixelr"
#define PAD_WIDTH "<PadWidth>"
#define PAD_HEIGHT "<PadHeight>"
#define MULTIPLICATIVE_OP "<MultiplicativeOp>"
#define ADDITIVE_OP "<AdditiveOp>"
#define RELATIONAL_OP "<RelationalOp>"
#define IDENTIFIER "<Identifier>"
#define END_OF_FILE "<EOF>"

using namespace std;


class Lexer {
public:
    Lexer() = default;
    void LexerPassFile(const string& path);
    vector<string> getRevisedTokens();
    vector<string> getProgramVector();
    string getProgramString();
private:
    vector<string> revisedTokens;
    vector<string> programVector;
    string programString;
    static vector<string> split(const string &s, char delimiter);
    static vector<string> splitLine(const string& s, const string& delimiters);
    static vector<string> loadProgramFile(const string& path);
    static string vectorToString(const vector<string>& programTokens);
    static map<string,map<string,string>> loadMap();
    static map<string, string> loadFinalStates();
    static string checkValidToken(char value);
    static vector<string> getListOfTokens(string line);
    static vector<string> traverseDFSA(map<string,map<string,string>> lexerTable, map<string, string> finalStates, vector<string> allLetters);
    static vector<string> reviseTokens(const vector<string>& allTokens, vector<string> allWords);
};
#endif //UNTITLED_LEXER_H
