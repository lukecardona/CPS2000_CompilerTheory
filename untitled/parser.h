#ifndef UNTITLED_PARSER_H
#define UNTITLED_PARSER_H

#include "lexer.h"

#define NULL_PRODUCTION "<Null>"
#define PROGRAM "<Program>"
#define STATEMENT "<Statement>"
#define VARIABLE_DECL "<VariableDecl>"
#define ASSIGNMENT "<Assignment>"
#define PRINT_STATEMENT "<PrintStatement>"
#define DELAY_STATEMENT "<DelayStatement>"
#define PIXEL_STATEMENT "<PixelStatement>"
#define IF_STATEMENT "<IfStatement>"
#define FOR_STATEMENT "<ForStatement>"
#define WHILE_STATEMENT "<WhileStatement>"
#define RETURN_STATEMENT "<RtrnStatement>"
#define FUNCTION_DECL "<FunctionDecl>"
#define BLOCK "<Block>"
#define EXPRESSION "<Expr>"
#define SIMPLE_EXPRESSION "<SimpleExpr>"
#define TERM "<Term>"
#define FACTOR "<Factor>"
#define LITERAL "<Literal>"
#define FUNCTION_CALL "<FunctionCall>"
#define SUB_EXPRESSION "<SubExpr>"
#define UNARY "<Unary>"
#define PAD_READ "<PadRead>"
#define PAD_RANDI "<PadRandI>"
#define ACTUAL_PARAMS "<ActualParams>"
#define FORMAL_PARAMS "<FormalParams>"
#define FORMAL_PARAM "<FormalParam>"
#define END_OF_FILE "<EOF>"
#define LET_TOKEN "<let>"
#define IF_TOKEN "<if>"
#define ELSE_TOKEN "<else>"
#define FOR_TOKEN "<for>"
#define WHILE_TOKEN "<while>"
#define NOT_UNARY_TOKEN "<Not>"
#define RETURN_TOKEN "<return>"
#define FUN_TOKEN "<fun>"

class ASTNode {
public:
    // Constructor
    explicit ASTNode(string  nodeType) : nodeType(std::move(nodeType)) {}

    // Get node type
    const string& getNodeType() const;

    // Add a child to the node
    virtual void addChild(unique_ptr<ASTNode> child);

    // Get the children of the node
    const vector<unique_ptr<ASTNode>>& getChildren() const;

    // Virtual destructor for proper cleanup in derived classes
    virtual ~ASTNode() = default;

protected:
    string nodeType; // The type of the node (e.g., "Expression", "Term", "Factor", etc.)
    vector<unique_ptr<ASTNode>> children; // Children of the node
};

class ProgramNode : public ASTNode {
public:
    // Constructor
    ProgramNode() : ASTNode(PROGRAM) {}
};
class StatementNode : public ASTNode {
public:
    explicit StatementNode() : ASTNode(STATEMENT) {}
};
class BlockNode : public ASTNode {
public:
    // Constructor
    BlockNode() : ASTNode(BLOCK) {}
};
class FunctionDeclNode : public ASTNode {
public:
    FunctionDeclNode() : ASTNode(FUNCTION_DECL) {}
};
class FormalParamsNode : public ASTNode {
public:
    FormalParamsNode() : ASTNode(FORMAL_PARAMS) {}
};
class FormalParamNode : public ASTNode{
public:
    FormalParamNode() : ASTNode(FORMAL_PARAM) {}
};
class WhileStatementNode : public ASTNode{
public:
    WhileStatementNode() : ASTNode(WHILE_STATEMENT) {}
};
class ForStatementNode : public ASTNode{
public:
    ForStatementNode() : ASTNode(FOR_STATEMENT) {}
};
class IfStatementNode : public ASTNode  {
public:
    IfStatementNode() : ASTNode(IF_STATEMENT) {}
};
class RtrnStatementNode : public ASTNode{
public:
    RtrnStatementNode() : ASTNode(RETURN_STATEMENT) {}
};

class PixelStatementNode : public ASTNode {
public:
    PixelStatementNode() : ASTNode(PIXEL_STATEMENT) {
        children.push_back(make_unique<ASTNode>(SPECIAL_PIXELR));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
        children.push_back(make_unique<ASTNode>(SEMICOLON_TOKEN));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
        children.push_back(make_unique<ASTNode>(SEMICOLON_TOKEN));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
        children.push_back(make_unique<ASTNode>(SEMICOLON_TOKEN));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
        children.push_back(make_unique<ASTNode>(SPECIAL_PIXEL));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
        children.push_back(make_unique<ASTNode>(SEMICOLON_TOKEN));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
        children.push_back(make_unique<ASTNode>(SEMICOLON_TOKEN));
        children.push_back(make_unique<ASTNode>(EXPRESSION));
    }
};
class DelayStatementNode : public ASTNode {
public:
    DelayStatementNode() : ASTNode(DELAY_STATEMENT) {}
};
class PrintStatementNode : public ASTNode {
public:
    PrintStatementNode() : ASTNode(PRINT_STATEMENT) {
    }
};
class AssignmentNode : public ASTNode {
public:
    AssignmentNode() : ASTNode(ASSIGNMENT) {}
};
class VariableDeclNode : public ASTNode {
public:
    VariableDeclNode() : ASTNode(VARIABLE_DECL) {}
};

class ExpressionNode : public ASTNode {
public:
    ExpressionNode() : ASTNode(EXPRESSION) {}
};
class SimpleExpressionNode : public ASTNode {
public:
    SimpleExpressionNode() : ASTNode(SIMPLE_EXPRESSION) {}
};
class TermNode : public ASTNode{
public:
    TermNode() : ASTNode(TERM) {}
};

class FactorNode : public ASTNode {
public:
    FactorNode() : ASTNode(FACTOR) {}
};
class UnaryNode : public ASTNode{
public:
    UnaryNode() : ASTNode(UNARY) {}
};
class SubExpressionNode : public ASTNode{
public:
    SubExpressionNode() : ASTNode(SUB_EXPRESSION) {}
};
class FunctionCallNode : public ASTNode {
public:
    FunctionCallNode() : ASTNode(FUNCTION_CALL) {}
};
class ActualParamsNode : public ASTNode{
public:
    ActualParamsNode() : ASTNode(ACTUAL_PARAMS) {}
};
class PadReadNode : public ASTNode {
public:
    PadReadNode() : ASTNode(PAD_READ) {}
};
class PadRandINode : public ASTNode {
public:
    PadRandINode() : ASTNode(PAD_RANDI) {}
};
class LiteralNode : public ASTNode {
public:
    LiteralNode() : ASTNode(LITERAL) {}
};

class Parser{
public:
    explicit Parser(vector<string> tk) : tokens(std::move(tk)) {}
    unique_ptr<ASTNode> parseProgram();
private:
    vector<string> tokens;
    int currentToken = 0;
    string currentState = tokens[0];
    stack<string> myStack;
    void consumeToken(const string& expectedToken);
    unique_ptr<ASTNode> parseProgramNode();
    unique_ptr<ASTNode> parseStatementNode();
    unique_ptr<ASTNode> parseBlockNode();
    unique_ptr<ASTNode> parseFunctionDeclNode();
    unique_ptr<ASTNode> parseFormalParamsNode();
    unique_ptr<ASTNode> parseWhileStatementNode();
    unique_ptr<ASTNode> parseForStatementNode();
    static unique_ptr<ASTNode> parseFormalParamNode();
    unique_ptr<ASTNode> parseIfStatementNode();
    unique_ptr<ASTNode> parseRtrnStatementNode();
    unique_ptr<ASTNode> parsePixelStatementNode();
    unique_ptr<ASTNode> parseDelayStatementNode();
    unique_ptr<ASTNode> parsePrintStatementNode();
    unique_ptr<ASTNode> parseAssignmentNode();
    unique_ptr<ASTNode> parseVariableDeclNode();
    unique_ptr<ASTNode> parseExpressionNode();
    unique_ptr<ASTNode> parseSimpleExpressionNode();
    unique_ptr<ASTNode> parseTermNode();
    unique_ptr<ASTNode> parseFactorNode();
    unique_ptr<ASTNode> parseUnaryNode();
    unique_ptr<ASTNode> parseSubExpressionNode();
    unique_ptr<ASTNode> parseFunctionCallNode();
    unique_ptr<ASTNode> parseActualParamsNode();
    unique_ptr<ASTNode> parsePadReadNode();
    unique_ptr<ASTNode> parsePadRandINode();
    unique_ptr<ASTNode> parseLiteralNode();
};

#endif //UNTITLED_PARSER_H
