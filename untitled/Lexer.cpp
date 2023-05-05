#include "header.h"

//Function to split the vector into data
std::vector<std::string> split(const std::string &s, char delimiter) { //Custom Function
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

map<string,map<string,string>> loadMap(){
    map<string,map<string,string>> lexerTable; //The map that will hold sub maps
    bool headerRow = true; //Flag to check for first row
    int counter;

    ifstream file("../Table Driven Lexer - Sheet1.csv"); //Open the CSV File
    if (!file.is_open()) { //Error Message in case file cant be open
        cerr << "Error: Unable to open the file." << std::endl;
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

map<string, string> loadFinalStates(){
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
            {"40", "}"}
    };
    return finalStates;
}//Load Final States

string checkValidToken(char value){

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

    if((value == MULTIPLY_TOKEN) || (value == DIVISION_TOKEN) || (value == ADDITION_TOKEN) || (value == SUBTRACTION_TOKEN)
    || (value == LESS_THAN_TOKEN) || (value == GREATER_THAN_TOKEN) || (value == EQUALS_TOKEN) || (value == NOT_TOKEN)
    || (value == OPEN_BRACKET_TOKEN) || (value == CLOSE_BRACKET_TOKEN) || (value == HASH_TOKEN) || (value == UNDERSCORE_TOKEN)
    || (value == DOT_TOKEN) || (value == OPEN_CURLY_BRACKET_TOKEN) || (value == CLOSE_CURLY_BRACKET_TOKEN) || (value == COLON_TOKEN)
    || (value == SEMICOLON_TOKEN))
    {
        string str;
        return str+=value;
    }

    return INVALID_TOKEN;
}//End Get Token of a character

vector<string> getListOfTokens(string line) {
    line += " ";
    vector<string> tokens;
    string token;
    for (char c: line) {
        token = checkValidToken(c);
        if (token == INVALID_TOKEN){
            //cerr << "Token Unrecognized";
        }
        tokens.push_back(token);
    }
    return tokens;
}

vector<string> traverseDFSA(map<string,map<string,string>> lexerTable, map<string, string> finalStates, vector<string> allLetters){
    string currentState = START_STATE;
    bool delimFlag = true;
    vector<string> tokens;
    string token;


    for (const std::string& letter : allLetters) {

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
                cerr << "Error: Key '" << currentState << "' not a final state. Exception message: " << e.what() << endl;
                tokens.emplace_back(INVALID_TOKEN); //Adds invalid token
                delimFlag = true; //Delim Flag is true
                currentState = START_STATE;
            }
            continue; //SKIP Transition
        }

        delimFlag = false; //no longer dealing with delimiters

        currentState = lexerTable[currentState][letter]; //Transition to the next state

    }
    return tokens;
}

vector<string> reviseTokens(const vector<string>& allTokens, vector<string> allWords){
    int counter = -1;
    vector<string> revisedToken(allTokens);
    string word;
    for (const std::string& token : allTokens) {
        counter += 1; //Increment Counter
        word = allWords[counter];
       if(token == COLOR_LITERAL){
           for (char c : allWords[counter]) {
               if (!((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c == '#') || (isdigit(c)))) { //If Not a valid Color literal raise and error
                   cerr << c << " is not a valid HEX Value" << endl;
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

           cerr << "Invalid Special Character trying to be accessed" << endl;
       }//End Check for valid Special Var

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
               revisedToken[counter] = word;
               continue;
           }

           bool correctWord = true;
           for (char c : word){
               if ( !( (isalpha(c)) || c == '_') ) { //If Not a valid Color literal raise and error
                   correctWord = false;
               } //End Check for valid hex literal
           } //End Loop To check for identifier

           if (correctWord){
               revisedToken[counter] = IDENTIFIER;
           }else{
               cerr << "Incorrect Format for identifier";
           }
           continue;
       } //Handle the word Token

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
    }//Loop through all tokens

    return revisedToken;
}//End DealWithWords