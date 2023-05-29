#include "lexer.h"
#include "ErrorHandling.h"

vector<string> Lexer::split(const string &s, char delimiter) { //Custom Function
    vector<std::string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
vector<string> Lexer::splitLine(const string& s, const string& delimiters) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    bool multiLineComment = false;

    const char *delimiterChars = delimiters.c_str();
    while (getline(tokenStream, token)) {
        size_t start = 0;
        size_t end = token.find_first_of(delimiterChars);
        while (end != string::npos) {
            if (end != start) {
                tokens.push_back(token.substr(start, end - start));
            }

            //Handles single Line Comments
            if (end < token.length() - 1 && (token.substr(end, 2) == "//")){
                end = string::npos;
                continue;
            }

            // Check if it's a part of a specific combination and handle accordingly
            if (end < token.length() - 1 && (token.substr(end, 2) == "==" || token.substr(end, 2) == "!=" ||
                                             token.substr(end, 2) == ">=" || token.substr(end, 2) == "<=" ||
                                             token.substr(end, 2) == "->")) {
                // Exclude space from delimiter list and add the delimiter as a separate token
                if (token[end] != ' ' && token[end + 1] != ' ') {
                    tokens.push_back(token.substr(end, 2));
                }
                start = end + 2;
            } else {

                if (token[end] != ' ') {
                    tokens.push_back(token.substr(end, 1));
                }
                start = end + 1;
            }

            end = token.find_first_of(delimiterChars, start);
        }
        // Push the remaining part of the token, if any
        if (start < token.length()) {
            tokens.push_back(token.substr(start));
        }
    }
    return tokens;
}
vector<string> Lexer::loadProgramFile(const string& path){
    vector<string> tokens;
    string line;
    vector<string> lineTokens;
    bool inComment = false;

    fstream file(path);
    if (!file.is_open()) {
        cerr << "\033[1;31m Failed to open the Program File. \033[0m" << endl;
        terminate();
    }

    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Remove carriage return ('\r') if present
        }

        // Handle multi-line comments
        if (!inComment && line.find("/*") != string::npos) {
            inComment = true;
            line = line.substr(0, line.find("/*"));
        }

        if (inComment && line.find("*/") != string::npos) {
            inComment = false;
            line = line.substr(line.find("*/") + 2);
        }

        if (inComment) {
            continue;
        }

        // Handle single line comments
        if (line.find("//") != string::npos) {
            line = line.substr(0, line.find("//"));
        }

        // Ignore the line if it is now empty
        if (line.empty())
            continue;

        lineTokens = splitLine(line, DELIMITERS);
        tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
    }
    file.close();

    // Unhandled multi-line comment error
    if(inComment){
        cerr << "\033[1;31m Program cannot end on an open multi line comment!! \033[0m" << endl;
        terminate();
    }

    //Print the loaded lines -> Uncomment for debugging
//    for (const std::string &str: tokens) {
//        cout << str << endl;
//    }

    return tokens;
}
string Lexer::vectorToString(const vector<string>& programTokens) {
    ostringstream oss;
    bool firstElement = true;

    for (const string& element : programTokens) {
        if (!firstElement) {
            oss << " "; // Add a space separator
        }
        oss << element;
        firstElement = false;
    }
    return oss.str();
}
map<string,map<string,string>> Lexer::loadMap(){
    map<string,map<string,string>> lexerTable; //The map that will hold sub maps
    bool headerRow = true; //Flag to check for first row
    int counter;

    ifstream file("../Table Driven Lexer - Sheet1.csv"); //Open the CSV File
    if (!file.is_open()) { //Error Message in case file cant be open

        cerr << "\033[1;31m FILE ERROR: Unable to open the file.\033[0m" << std::endl;
        terminate();
        return lexerTable;
    }

    string line; //Holds the entire line
    string token;
    vector<string> secondColumnValues; //Will hold the final states
    vector<string> headerValues; //Will hold the header values used to build a map of maps

    while (getline(file, line)) { //Get lines
        vector<string> row = split(line, ','); //Split line by Comma

        //If First Row
        if(headerRow){
            headerRow = false; //No longer first row
            for (size_t i = 3; i < row.size(); i++) {
                headerValues.push_back(row[i]); //Push back the first row values
            }
        }
        else {
            if(row[1] == FINAL_STATE) { //If it's a final state add it to final states.
                secondColumnValues.push_back(row[2]);
            }

            map<string,string> subMap;
            token = row[2];
            counter = 0;
            for (size_t i = 3; i < row.size(); i++) {
                subMap[headerValues[counter]] = row[i];
                counter += 1;
            }
            lexerTable[token] = subMap;
        }
    }
    file.close(); //Closes the file
    lexerTable.erase("");
    return lexerTable; //Return the Built lexer Table
} //End Load Map
map<string, string> Lexer::loadFinalStates(){
    map<string, string> finalStates = {
            {"8", COLOR_LITERAL},
            {"14", SPECIAL_VAR},
            {"15", SPECIAL_VAR},
            {"16", SPECIAL_VAR},
            {"17", INTEGER_LITERAL},
            {"19", FLOAT_LITERAL},
            {"20", WORD},
            {"21", "*"},
            {"22", "/"},
            {"23", "+"},
            {"24", "-"},
            {"25", ">"},
            {"26", ">="},
            {"27", "<"},
            {"28", "<="},
            {"29", "="},
            {"30", "=="},
            {"32", "!="},
            {"33", "("},
            {"34", ")"},
            {"35", ","},
            {"36", ":"},
            {"37", ";"},
            {"38", "->"},
            {"39", "{"},
            {"40", "}"},
            {"41", "//"},
            {"42", "/*"},
            {"43","*/"}
    };
    return finalStates;
}//Load Final States
string Lexer::checkValidToken(char value){

    if(value == ','){
        return COMMA_TOKEN;
    }

    if(isdigit(value)){
        return DIGIT_TOKEN;
    }

    if(isalpha(value)){
        return LETTER_TOKEN;
    }

    if (value == ' '){
        return EMPTY_SPACE_TOKEN;
    }

    if((value == MULTIPLY_VALUE) || (value == DIVISION_VALUE) || (value == ADDITION_VALUE) || (value == SUBTRACTION_VALUE)
       || (value == LESS_THAN_VALUE) || (value == GREATER_THAN_VALUE) || (value == EQUALS_VALUE) || (value == NOT_VALUE)
       || (value == OPEN_BRACKET_VALUE) || (value == CLOSE_BRACKET_VALUE) || (value == HASH_VALUE) || (value == UNDERSCORE_VALUE)
       || (value == DOT_VALUE) || (value == OPEN_CURLY_BRACKET_VALUE) || (value == CLOSE_CURLY_BRACKET_VALUE) || (value == COLON_VALUE)
       || (value == SEMICOLON_VALUE))
    {
        string str;
        str += value;
        return str;
    }

    return INVALID_TOKEN;
}//End Get Token of a character
vector<string> Lexer::getListOfTokens(string line) {
    line += " ";
    vector<string> tokens;
    string token;
    bool minus = false;

    for (char c: line) {
        token = checkValidToken(c);
        if (token == INVALID_TOKEN){
            //Throw Error that character is unrecognized
            try{
                string errorMsg = UNRECOGNIZED_CHAR_ERROR;
                errorMsg.push_back(c);
                throw UnrecognizedCharacter(errorMsg);
            }
            catch (const exception& e){
                cerr << "\033[1;31mUNRECOGNIZED Character: " << e.what() << "\033[0m" << endl;
                terminate();
            }
        }

        tokens.push_back(token);
    }
    return tokens;
}
vector<string> Lexer::traverseDFSA(map<string,map<string,string>> lexerTable, map<string, string> finalStates, vector<string> allLetters){
    string currentState = START_STATE;
    bool delimFlag = true;
    vector<string> tokens;
    string token;
    int counter = -2;


    for (const std::string& letter : allLetters) {
        counter += 1;
        if(letter == EMPTY_SPACE_TOKEN && delimFlag){ //Handles delim after delim
            continue; //SKIP Transition
        }

        if(letter == EMPTY_SPACE_TOKEN && !delimFlag){ //Checks if state is a final state
            //Check which final state
            try {
                token = finalStates.at(currentState); //If token is in a final State
                tokens.push_back(token); //Push Back Token
                delimFlag = true; //Delim Flag is true
                currentState = START_STATE; //Set current state to initial state
            } catch (const out_of_range& e) {
                cerr << "\033[1;31m INVALID STATE at: '" << allLetters[counter] << "' not a final state.\033[0m \n Exception message:" << e.what() << endl;
                tokens.emplace_back(INVALID_TOKEN); //Adds invalid token
                delimFlag = true; //Delim Flag is true
                currentState = START_STATE;
                terminate();
            }
            continue; //SKIP Transition
        }

        delimFlag = false; //no longer dealing with delimiters

        currentState = lexerTable[currentState][letter]; //Transition to the next state
    }
    return tokens;
}
vector<string> Lexer::reviseTokens(const vector<string>& allTokens, vector<string> allWords){
    int counter = -1;
    vector<string> revisedToken(allTokens);
    string word;
    for (const std::string& token : allTokens) {
        counter += 1; //Increment Counter
        word = allWords[counter];
        if(token == COLOR_LITERAL){
            for (char c : allWords[counter]) {
                if (!((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c == '#') || (isdigit(c)))) { //If Not a valid Color literal raise and error
                    cerr << "\033[1;31m INVALID HEX: '" << c << "' not a valid Hex Value.\033[0m \n Exception message:" << endl;
                    terminate();
                } //End Check for valid hex literal
            } //End Loop through colorLiteral
            continue;
        } //End Check for valid colorLiteral

        if(token == SPECIAL_VAR){

            //Check all Special Var
            if(word == SPECIAL_WIDTH){
                revisedToken[counter] = PAD_WIDTH;
                continue;
            }

            if(word == SPECIAL_HEIGHT){
                revisedToken[counter] = PAD_HEIGHT;
                continue;
            }

            //Unable to recurse back to full tokens
            if((word == SPECIAL_RANDI) || (word == SPECIAL_READ) || (word == SPECIAL_PRINT) ||
               (word == SPECIAL_DELAY) || (word == SPECIAL_PIXEL) || (word == SPECIAL_PIXELR)){
                revisedToken[counter] = word;
                continue;
            }

            cerr << "\033[1;31m INVALID SPECIAL VARIABLE: '" << word << "' not a valid Special variable.\033[0m\n" << endl;
        }//End Check for valid Special Var

        //Handle Operators
        if ((token == "*") || (token == "/")){
            revisedToken[counter] = MULTIPLICATIVE_OP;
            continue;
        }

        if ((token == "+") || (token == "-")){
            revisedToken[counter] = ADDITIVE_OP;
            continue;
        }

        if ((token == ">") || (token == "<") || (token == ">=") || (token == "<=") || (token == "==") || (token == "!=")){
            revisedToken[counter] = RELATIONAL_OP;
            continue;
        }

//        if ((token == "->")){
//            revisedToken[counter] = RELATIONAL_OP;
//            continue;
//        }

        if ((token == ",")){
            revisedToken[counter] = COMMA_TOKEN;
            continue;
        }

        if ((token == ".")){
            revisedToken[counter] = DOT_TOKEN;
            continue;
        }

        if((token == ";")){
            revisedToken[counter] = SEMICOLON_TOKEN;
            continue;
        }

        if((token == ":")){
            revisedToken[counter] = COLON_TOKEN;
            continue;
        }

        if ((token == "{")){
            revisedToken[counter] = OPEN_CURLY_BRACKET_TOKEN;
            continue;
        }

        if ((token == "}")){
            revisedToken[counter] = CLOSE_CURLY_BRACKET_TOKEN;
            continue;
        }

        if ((token == "(")){
            revisedToken[counter] = OPEN_BRACKET_TOKEN;
            continue;
        }

        if ((token == ")")){
            revisedToken[counter] = CLOSE_BRACKET_TOKEN;
            continue;
        }

        if ((token == "=")){
            revisedToken[counter] = EQUALS_TOKEN;
            continue;
        }

        if(token == WORD){
            if( (word == "float") || (word == "int") || (word == "bool") || (word == "colour") ){
                revisedToken[counter] = TYPE;
                continue;
            }

            if ( (word == "false") || (word == "true") ){
                revisedToken[counter] = BOOLEAN_LITERAL;
                continue;
            }

            if (word  == "and"){
                revisedToken[counter] = MULTIPLICATIVE_OP;
                continue;
            }

            if (word == "or"){
                revisedToken[counter] = ADDITIVE_OP;
                continue;
            }

            if ((word == "let") || (word == "if") || (word == "else") || (word == "for") || (word == "while") ||
                (word == "fun") || (word == "return") || (word == "not")){
                revisedToken[counter] = "<"+word+">";
                continue;
            }

            bool correctWord = true;
            for (char c : word){
                if ( !( (isalpha(c)) || c == '_') ) { //If Not a valid Word
                    correctWord = false;
                }
            } //End Loop To check for identifier

            if (correctWord){
                revisedToken[counter] = IDENTIFIER;
            }else{
                cerr << "\033[1;31m UNRECOGNIZED WORD: '" << word << "' not a valid Identifier.\033[0m \n" << endl;
                terminate();
            }
            continue;
        } //Handle the word Token


    }//Loop through all tokens

    return revisedToken;
}//End DealWithWords
void Lexer::LexerPassFile(const string& path) {
    programVector = loadProgramFile(path);
    programString = vectorToString(programVector);
    vector<string> programTokenizedLetters = getListOfTokens(programString);
    //Uncomment to test tokenized Letters
//    for (const std::string& str : programTokenizedLetters) {
//        std::cout << str << " ";
//    }
    map<string, map<string, string>> lexerTable = loadMap();
    map<string, string> finalStates = loadFinalStates();
    vector<string> allTokens = traverseDFSA(lexerTable, finalStates, programTokenizedLetters);
    //Uncomment to test traversed DFSA
//    for (const std::string& str : allTokens) {
//        std::cout << str << " ";
//    }

    //Uncomment to test revised tokens
    revisedTokens = reviseTokens(allTokens, programVector);
//    for (const std::string& str : revisedTokens) {
//        std::cout << str << " ";
//    }
    revisedTokens.emplace_back(END_OF_FILE);
    //Uncomment to test program string being generated
    //cout << programString << endl;
}
vector<string> Lexer::getProgramVector() {
    return programVector;
}
vector<string> Lexer::getRevisedTokens() {
    return revisedTokens;
}
string Lexer::getProgramString() {
    return programString;
}