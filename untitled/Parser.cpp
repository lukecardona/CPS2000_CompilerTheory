#include <utility>

#include "header.h"

#define LET "let"

#define DEFAULT_START_VALUE "start"
#define NULL_PRODUCTION "<Null>"

#define PROGRAM "<Program>"
#define PROGRAM_TAIL "<Program_Tail>"
#define STATEMENT "<Statement>"

#define VARIABLE_DECL "<VariableDecl>"
#define ASSIGNMENT "<Assignment>"
#define PRINT_STATEMENT "<PrintStatement>"
#define DELAY_STATEMENT "<DelayStatement>"
#define PIXEL_STATEMENT "<PixelStatement>"
#define IF_STATEMENT "<IfStatement>"
#define IF_STATEMENT_TAIL "<IfStatementTail>"
#define FOR_STATEMENT "<ForStatement>"
#define FOR_STATEMENT_TAIL "<ForStatementTail>"
#define FOR_STATEMENT_TAIL2 "<ForStatementTail2>"
#define WHILE_STATEMENT "<WhileStatement>"
#define RETURN_STATEMENT "<RtrnStatement>"
#define FUNCTION_DECL "<FunctionDecl>"
#define FUNCTION_DECL_TAIL "<FunctionDeclTail>"
#define BLOCK "<Block>"

#define EXPRESSION "<Expr>"
#define EXPRESSION_TAIL "<ExprTail>"
#define SIMPLE_EXPRESSION "<SimpleExpr>"
#define SIMPLE_EXPRESSION_TAIL "<SimpleExprTail>"
#define TERM "<Term>"
#define TERM_TAIL "<TermTail>"
#define FACTOR "<Factor>"
#define LITERAL "<Literal>"
#define FUNCTION_CALL "<FunctionCall>"
#define FUNCTION_CALL_TAIL "<FunctionCallTail>"
#define SUB_EXPRESSION "<SubExpr>"
#define UNARY "<Unary>"
#define PAD_READ "<PadRead>"
#define PAD_RANDI "<PadRandI>"
#define ACTUAL_PARAMS "<ActualParams>"
#define ACTUAL_PARAMS_TAIL "<ActualParamsTail>"
#define FORMAL_PARAMS "<FormalParams>"
#define FORMAL_PARAMS_TAIL "<FormalParamsTail>"
#define FORMAL_PARAM "<FormalParam>"

const map<std::pair<std::string, std::string>, std::vector<std::string>> parseTable{
        //Program
        {{PROGRAM, DEFAULT_START_VALUE}, {STATEMENT, PROGRAM_TAIL}},
        {{PROGRAM_TAIL, STATEMENT},{STATEMENT,PROGRAM_TAIL}},
        {{PROGRAM_TAIL,NULL_PRODUCTION},{NULL_PRODUCTION}},
        //Statements
        {{STATEMENT, LET}, {VARIABLE_DECL, ";"}},
        {{STATEMENT, IDENTIFIER}, {ASSIGNMENT,";"}},
        {{STATEMENT, SPECIAL_PRINT}, {PRINT_STATEMENT, ";"}},
        {{STATEMENT, DELAY_STATEMENT}, {DELAY_STATEMENT,";"}},
        {{STATEMENT, SPECIAL_PIXELR}, {PIXEL_STATEMENT,";"}},
        {{STATEMENT, "if"}, {IF_STATEMENT}},
        {{STATEMENT, "for"}, {FOR_STATEMENT}},
        {{STATEMENT, "while"}, {WHILE_STATEMENT}},
        {{STATEMENT, "return"}, {RETURN_STATEMENT,";"}},
        {{STATEMENT, "fun"}, {FUNCTION_DECL}},
        {{STATEMENT, "{"}, {BLOCK}},

         //Block
         {{BLOCK,"{"},{"{",BLOCK,"}"}},

        //Variable Declaration
        {{VARIABLE_DECL, LET}, {LET, IDENTIFIER, ":",TYPE,"=",EXPRESSION}},
        //Type Declaration
        {{TYPE, "float"}, {FLOAT_LITERAL}},
        {{TYPE, "int"}, {INTEGER_LITERAL}},
        {{TYPE, "bool"}, {BOOLEAN_LITERAL}},
        {{TYPE, "colour"}, {COLOR_LITERAL}},
        //Assignment Declaration
        {{ASSIGNMENT,IDENTIFIER},{IDENTIFIER,"=",EXPRESSION}},
        //Print Declaration
        {{PRINT_STATEMENT,SPECIAL_PRINT},{SPECIAL_PRINT,EXPRESSION}},
        //Delay Statement
        {{DELAY_STATEMENT,SPECIAL_DELAY},{SPECIAL_DELAY,EXPRESSION}},
        //Special Pixel Statement
        {{PIXEL_STATEMENT,SPECIAL_PIXELR},{SPECIAL_PIXELR,EXPRESSION,",",EXPRESSION,",",EXPRESSION,",",
                                           EXPRESSION,",",EXPRESSION,SPECIAL_PIXEL,EXPRESSION,",",EXPRESSION,
                                           ",",EXPRESSION}},

        //If Statement
        {{IF_STATEMENT,"if"},{"if","(",EXPRESSION,")",BLOCK,IF_STATEMENT_TAIL}},
        {{IF_STATEMENT_TAIL,NULL_PRODUCTION},{NULL_PRODUCTION}},
        {{IF_STATEMENT_TAIL,"else"},{"else",BLOCK}},
        //For Statement
        {{FOR_STATEMENT,"for"},{"for","(",FOR_STATEMENT_TAIL,EXPRESSION,";",FOR_STATEMENT_TAIL2,BLOCK}},
        {{FOR_STATEMENT_TAIL,";"},{";"}},
        {{FOR_STATEMENT_TAIL,VARIABLE_DECL},{VARIABLE_DECL,";"}},
        {{FOR_STATEMENT_TAIL2,")"},{")"}},
        {{FOR_STATEMENT_TAIL2,IDENTIFIER},{ASSIGNMENT,")"}},
        //While Statement
        {{WHILE_STATEMENT,"while"},{"while","(",EXPRESSION,")",BLOCK}},
        //Return Statement
        {{RETURN_STATEMENT,"return"},{"return",EXPRESSION}},
        //Function Declare
        {{FUNCTION_DECL,"fun"},{"fun",IDENTIFIER,"(",FUNCTION_DECL_TAIL,"->",TYPE,BLOCK}},
        {{FUNCTION_CALL_TAIL,")"},{")"}},
        {{FUNCTION_CALL_TAIL,IDENTIFIER},{FORMAL_PARAMS,")"}},
        //Formal Params
        {{FORMAL_PARAMS,IDENTIFIER},{FORMAL_PARAM,FORMAL_PARAMS_TAIL}},
        {{FOR_STATEMENT_TAIL,NULL_PRODUCTION},{NULL_PRODUCTION}},
        {{FORMAL_PARAMS_TAIL,","},{",",FORMAL_PARAM,FORMAL_PARAMS_TAIL}},
        //Formal Param
        {{FORMAL_PARAM,IDENTIFIER},{IDENTIFIER,":",TYPE}},
        //Expression
        {{EXPRESSION, SIMPLE_EXPRESSION}, {SIMPLE_EXPRESSION, EXPRESSION_TAIL}},
        {{EXPRESSION_TAIL, NULL_PRODUCTION}, {NULL_PRODUCTION}},
        {{EXPRESSION_TAIL, RELATIONAL_OP}, {RELATIONAL_OP, SIMPLE_EXPRESSION, EXPRESSION_TAIL}},
        //Simple Expression
        {{SIMPLE_EXPRESSION, TERM}, {TERM, SIMPLE_EXPRESSION_TAIL}},
        {{SIMPLE_EXPRESSION_TAIL, NULL_PRODUCTION}, {NULL_PRODUCTION}},
        {{SIMPLE_EXPRESSION_TAIL, ADDITIVE_OP}, {ADDITIVE_OP, TERM, SIMPLE_EXPRESSION_TAIL}},
        //Term
        {{TERM, FACTOR}, {FACTOR, TERM_TAIL}},
        {{TERM_TAIL, NULL_PRODUCTION}, {NULL_PRODUCTION}},
        {{TERM, MULTIPLICATIVE_OP}, {MULTIPLICATIVE_OP, FACTOR, TERM_TAIL}},
        //Factor
        {{FACTOR, LITERAL}, {LITERAL}},
        {{FACTOR, IDENTIFIER}, {IDENTIFIER}},
        {{FACTOR, FUNCTION_CALL}, {FUNCTION_CALL}},
        {{FACTOR, "("},{SUB_EXPRESSION}},
        {{FACTOR, "-"},{UNARY}},
        {{FACTOR, "not"},{UNARY}},
        {{FACTOR,PAD_WIDTH},{PAD_WIDTH}},
        {{FACTOR,PAD_HEIGHT},{PAD_HEIGHT}},
        {{FACTOR,SPECIAL_READ},{PAD_READ}},
        {{FACTOR,SPECIAL_RANDI},{PAD_RANDI}},
        //Literals
        {{LITERAL, BOOLEAN_LITERAL}, {BOOLEAN_LITERAL}},
        {{LITERAL, INTEGER_LITERAL}, {INTEGER_LITERAL}},
        {{LITERAL, FLOAT_LITERAL}, {FLOAT_LITERAL}},
        {{LITERAL, COLOR_LITERAL}, {COLOR_LITERAL}},
        {{LITERAL, PAD_WIDTH}, {PAD_WIDTH}},
        {{LITERAL, PAD_HEIGHT}, {PAD_HEIGHT}},
        {{LITERAL, SPECIAL_READ}, {PAD_READ}},
        //FunctionCall
        {{FUNCTION_CALL,IDENTIFIER},{IDENTIFIER,"(",FUNCTION_CALL_TAIL}},
        {{FUNCTION_CALL_TAIL,")"},{")"}},
        {{FUNCTION_CALL_TAIL,EXPRESSION},{ACTUAL_PARAMS,")"}},
        //Actual Params
        {{ACTUAL_PARAMS,EXPRESSION},{EXPRESSION,ACTUAL_PARAMS_TAIL}},
        {{ACTUAL_PARAMS_TAIL,")"},{NULL_PRODUCTION}},
        {{ACTUAL_PARAMS_TAIL,","},{",",EXPRESSION,ACTUAL_PARAMS_TAIL}},
        //SubExpression
        {{SUB_EXPRESSION, "("},{"(",EXPRESSION,")"}},
        //Unary
        {{UNARY,"-"},{"-",EXPRESSION}},
        {{UNARY,"not"},{"not",EXPRESSION}},
        //SpecialVars
        {{PAD_READ,SPECIAL_READ},{SPECIAL_READ,EXPRESSION,",",EXPRESSION}},
        {{PAD_RANDI,SPECIAL_RANDI},{SPECIAL_RANDI,EXPRESSION}},
};

class ASTNode {
    public:
    // Constructor
    explicit ASTNode(string  nodeType) : nodeType(std::move(nodeType)) {}

    // Get node type
    const string& getNodeType() const {
        return nodeType;
    }

    // Add a child to the node
    void addChild(unique_ptr<ASTNode> child) {
        children.push_back(std::move(child));
    }

    // Get the children of the node
    const std::vector<unique_ptr<ASTNode>>& getChildren() const {
        return children;
    }

    // Virtual destructor for proper cleanup in derived classes
    virtual ~ASTNode() = default;

    private:
    std::string nodeType; // The type of the node (e.g., "Expression", "Term", "Factor", etc.)
    std::vector<std::unique_ptr<ASTNode>> children; // Children of the node
};







int main(){
    string testString = "int x = 10";
    cout << "Testing: " << testString << endl;
    map<string,map<string,string>> lexerTable = loadMap(); //Loads the map from the CSV file
    map<string, string> finalStates = loadFinalStates();
    vector<string> allLetters = getListOfTokens(testString);
    vector<string> allWords = split(testString,' ');
    vector<string> allTokens = traverseDFSA(lexerTable,finalStates,allLetters);
    vector<string> revisedTokens = reviseTokens(allTokens,allWords);

//    // Iterate through the nested map and print key-value pairs
//    for (const auto &outer_pair : lexerTable) {
//        cout << "Outer Key: " << outer_pair.first << std::endl;
//
//        for (const auto &inner_pair : outer_pair.second) {
//            cout << "  Inner Key: " << inner_pair.first
//                      << ", Value: " << inner_pair.second << std::endl;
//        }
//    }

    for (const std::string& str : allTokens) {
        std::cout << str << " ";
    }
    cout << endl;

    for (const std::string& str : revisedTokens) {
        std::cout << str << " ";
    }
    cout << endl;
    return 0;
}