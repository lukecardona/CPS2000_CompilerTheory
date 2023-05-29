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

class BlockNode;
class ExpressionNode;
class ProgramNode;
class Visitor;

class ASTNode {
public:
    // Constructor
    explicit ASTNode(string  nodeType) : nodeType(std::move(nodeType)) {}

    // Get node type
    virtual const string& getNodeType() const;

    // Virtual destructor for proper cleanup in derived classes
    virtual ~ASTNode() = default;
    virtual void accept(Visitor* visitor) = 0;

    unique_ptr<ProgramNode> programNode; // Children of the node
protected:
    string nodeType; // The type of the node (e.g., "Expression", "Term", "Factor", etc.)
};

class StatementNode : public ASTNode {
public:
    StatementNode() : ASTNode(STATEMENT) {}
    const string& getNodeType() const override;
    void accept(Visitor* visitor) override;
};
class FactorNode : public ASTNode {
public:
    FactorNode() : ASTNode(FACTOR) {}
    const string& getNodeType() const override;
    void accept(Visitor* visitor) override;
};
class TypeNode : public ASTNode{
public:
    TypeNode() : ASTNode(TYPE) {}
    string value;
    void accept(Visitor* visitor) override;
};
class IdentifierNode : public FactorNode{
public:
    string value;
    IdentifierNode() : FactorNode() {}
    void accept(Visitor* visitor) override;
private:
    string nodeType = IDENTIFIER;
};
class RelationalOpNode : public ASTNode{
public:
    void accept(Visitor* visitor)override;
    RelationalOpNode() : ASTNode(RELATIONAL_OP) {}
    string value;
};
class AdditiveOpNode : public ASTNode{
public:
    void accept(Visitor* visitor) override;
    AdditiveOpNode() : ASTNode(ADDITIVE_OP) {}
    string value;
};
class MultiplicativeOpNode : public ASTNode{
public:
    void accept(Visitor* visitor) override;
    MultiplicativeOpNode() : ASTNode(MULTIPLICATIVE_OP) {}
    string value;
};
class LiteralNode : public FactorNode {
public:
    LiteralNode() : FactorNode() {}
    void accept(Visitor* visitor) override;
private:
    string nodeType = LITERAL;
};
class BooleanLiteralNode : public LiteralNode{
public:
    void accept(Visitor* visitor) override;
    BooleanLiteralNode() : LiteralNode() {}
    string value;
private:
    string nodeType = BOOLEAN_LITERAL;
};
class IntegerLiteralNode : public LiteralNode{
public:
    void accept(Visitor* visitor) override;
    IntegerLiteralNode() : LiteralNode() {}
    string value;
private:
    string nodeType = INTEGER_LITERAL;
};
class FloatLiteralNode : public LiteralNode{
public:
    void accept(Visitor* visitor) override;
    FloatLiteralNode() : LiteralNode() {}
    string value;
private:
    string nodeType = FLOAT_LITERAL;
};
class ColourLiteralNode : public LiteralNode{
public:
    void accept(Visitor* visitor) override;
    ColourLiteralNode() : LiteralNode() {}
    string value;
private:
    string nodeType = COLOR_LITERAL;
};
class PadWidthNode : public LiteralNode{
public:
    void accept(Visitor* visitor) override;
    PadWidthNode() : LiteralNode() {}
private:
    string nodeType = PAD_WIDTH;
};
class PadHeightNode : public LiteralNode{
public:
    void accept(Visitor* visitor) override;
    PadHeightNode() : LiteralNode() {}
private:
    string nodeType = PAD_HEIGHT;
};
class PadReadNode : public LiteralNode {
public:
    void accept(Visitor* visitor) override;
    PadReadNode() : LiteralNode() {}
    tuple<unique_ptr<ExpressionNode>,unique_ptr<ExpressionNode>> expressions;
private:
    string nodeType = PAD_READ;
};
class PadRandINode : public FactorNode {
public:
    void accept(Visitor* visitor) override;
    PadRandINode() : FactorNode() {}
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = PAD_RANDI;
};
class ActualParamsNode : public ASTNode{
public:
    void accept(Visitor* visitor) override;
    ActualParamsNode() : ASTNode(ACTUAL_PARAMS) {}
    unique_ptr<ExpressionNode> leftExpression;
    vector<unique_ptr<ExpressionNode>> rightExpressions;
};
class FunctionCallNode : public FactorNode {
public:
    void accept(Visitor* visitor) override;
    FunctionCallNode() : FactorNode() {}
    unique_ptr<IdentifierNode> identifier;
    unique_ptr<ActualParamsNode> actualParams = nullptr;
private:
    string nodeType = FUNCTION_CALL;
};
class SubExpressionNode : public FactorNode{
public:
    void accept(Visitor* visitor) override;
    SubExpressionNode() : FactorNode() {}
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = SUB_EXPRESSION;
};
class UnaryNode : public FactorNode{
public:
    void accept(Visitor* visitor) override;
    UnaryNode() : FactorNode() {}
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = UNARY;
};
class TermNode : public ASTNode {
public:
    void accept(Visitor* visitor) override;
    TermNode() : ASTNode(TERM) {}
    unique_ptr<FactorNode> leftFactor;
    vector<tuple<unique_ptr<MultiplicativeOpNode>,unique_ptr<FactorNode>>> rightFactor;
};
class SimpleExpressionNode : public ASTNode {
public:
    void accept(Visitor* visitor) override;
    SimpleExpressionNode() : ASTNode(SIMPLE_EXPRESSION) {}
    unique_ptr<TermNode> leftTerm;
    vector<tuple<unique_ptr<AdditiveOpNode>,unique_ptr<TermNode>>> rightTerm;
};
class ExpressionNode : public ASTNode {
public:
    void accept(Visitor* visitor) override;
    ExpressionNode() : ASTNode(EXPRESSION) {}
    unique_ptr<SimpleExpressionNode> leftSimpleExpression;
    vector<tuple<unique_ptr<RelationalOpNode>,unique_ptr<SimpleExpressionNode>>> rightSimpleExpression;
};
class AssignmentNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    AssignmentNode() : StatementNode() {}
    unique_ptr<IdentifierNode> identifier;
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = ASSIGNMENT;
};
class VariableDeclNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    VariableDeclNode() : StatementNode() {}
    unique_ptr<IdentifierNode> identifier;
    unique_ptr<TypeNode> type;
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = VARIABLE_DECL;
};
class PrintStatementNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    PrintStatementNode() : StatementNode() {}
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = PRINT_STATEMENT;
};
class DelayStatementNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    DelayStatementNode() : StatementNode() {}
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = DELAY_STATEMENT;
};
class PixelStatementNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    PixelStatementNode() : StatementNode() {}
    tuple<unique_ptr<ExpressionNode>,unique_ptr<ExpressionNode>,
    unique_ptr<ExpressionNode>,unique_ptr<ExpressionNode>,unique_ptr<ExpressionNode>>
    pixel_r = make_tuple(
            unique_ptr<ExpressionNode>{nullptr}, unique_ptr<ExpressionNode>{nullptr},
            unique_ptr<ExpressionNode>{nullptr}, unique_ptr<ExpressionNode>{nullptr},
            unique_ptr<ExpressionNode>{nullptr});

    tuple<unique_ptr<ExpressionNode>,unique_ptr<ExpressionNode>,
            unique_ptr<ExpressionNode>> pixel = make_tuple(
            unique_ptr<ExpressionNode>{nullptr}, unique_ptr<ExpressionNode>{nullptr},
            unique_ptr<ExpressionNode>{nullptr});
private:
    string nodeType = PIXEL_STATEMENT;
};
class ForStatementNode : public StatementNode{
public:
    void accept(Visitor* visitor) override;
    ForStatementNode() : StatementNode() {}
    unique_ptr<VariableDeclNode> varDecl = nullptr;
    unique_ptr<ExpressionNode> expression;
    unique_ptr<AssignmentNode> assignment = nullptr;
    unique_ptr<BlockNode> block;
private:
    string nodeType = FOR_STATEMENT;
};
class WhileStatementNode : public StatementNode{
public:
    void accept(Visitor* visitor) override;
    WhileStatementNode() : StatementNode() {}
    unique_ptr<ExpressionNode> expression;
    unique_ptr<BlockNode> block;
private:
    string nodeType = WHILE_STATEMENT;
};
class IfStatementNode : public StatementNode  {
public:
    void accept(Visitor* visitor) override;
    IfStatementNode() : StatementNode() {}
    unique_ptr<ExpressionNode> expression;
    unique_ptr<BlockNode> ifBlock;
    unique_ptr<BlockNode> elseBlock = nullptr;
private:
    string nodeType = IF_STATEMENT;
};
class RtrnStatementNode : public StatementNode{
public:
    void accept(Visitor* visitor) override;
    RtrnStatementNode() : StatementNode() {}
    unique_ptr<ExpressionNode> expression;
private:
    string nodeType = RETURN_STATEMENT;
};
class FormalParamNode : public ASTNode{
public:
    void accept(Visitor* visitor) override;
    FormalParamNode() : ASTNode(FORMAL_PARAM) {}
    unique_ptr<IdentifierNode> identifier;
    unique_ptr<TypeNode> type;
};
class FormalParamsNode : public ASTNode {
public:
    void accept(Visitor* visitor) override;
    FormalParamsNode() : ASTNode(FORMAL_PARAMS) {}
    unique_ptr<FormalParamNode> formalParamLeft;
    vector<unique_ptr<FormalParamNode>>  formalParamRight;
};
class FunctionDeclNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    FunctionDeclNode() = default;
    unique_ptr<IdentifierNode> identifier;
    unique_ptr<FormalParamsNode> formalParams = nullptr;
    unique_ptr<TypeNode> type;
    unique_ptr<BlockNode> block;

private:
    string nodeType = FUNCTION_DECL;
};
class BlockNode : public StatementNode {
public:
    void accept(Visitor* visitor) override;
    // Constructor
    BlockNode() : StatementNode() {}
    vector<unique_ptr<StatementNode>> statements;
private:
    string nodeType = BLOCK;
};
class ProgramNode : public ASTNode {
public:
    void accept(Visitor* visitor) override;
    // Constructor
    ProgramNode() : ASTNode(PROGRAM) {}
    vector<unique_ptr<StatementNode>> statements;
};

class Visitor {
public:
    virtual void VisitNode(ASTNode* node) = 0;
    virtual void VisitNode(ProgramNode* node) = 0;
    virtual void VisitNode(StatementNode* node) = 0;
    virtual void VisitNode(BlockNode* node) = 0;
    virtual void VisitNode(FunctionDeclNode* node) = 0;
    virtual void VisitNode(FormalParamsNode* node) = 0;
    virtual void VisitNode(FormalParamNode* node) = 0;
    virtual void VisitNode(WhileStatementNode* node) = 0;
    virtual void VisitNode(ForStatementNode* node) = 0;
    virtual void VisitNode(IfStatementNode* node) = 0;
    virtual void VisitNode(RtrnStatementNode* node) = 0;
    virtual void VisitNode(PixelStatementNode* node) = 0;
    virtual void VisitNode(DelayStatementNode* node) = 0;
    virtual void VisitNode(PrintStatementNode* node) = 0;
    virtual void VisitNode(AssignmentNode* node) = 0;
    virtual void VisitNode(VariableDeclNode* node) = 0;
    virtual void VisitNode(ExpressionNode* node) = 0;
    virtual void VisitNode(SimpleExpressionNode* node) = 0;
    virtual void VisitNode(TermNode* node) = 0;
    virtual void VisitNode(FactorNode* node) = 0;
    virtual void VisitNode(UnaryNode* node) = 0;
    virtual void VisitNode(SubExpressionNode* node) = 0;
    virtual void VisitNode(FunctionCallNode* node) = 0;
    virtual void VisitNode(ActualParamsNode* node) = 0;
    virtual void VisitNode(PadRandINode* node) = 0;
    virtual void VisitNode(PadReadNode* node) = 0;
    virtual void VisitNode(LiteralNode* node) = 0;
    virtual void VisitNode(IdentifierNode* node) = 0;
    virtual void VisitNode(TypeNode* node) = 0;
    virtual void VisitNode(RelationalOpNode* node) = 0;
    virtual void VisitNode(AdditiveOpNode* node) = 0;
    virtual void VisitNode(MultiplicativeOpNode* node) = 0;
    virtual void VisitNode(BooleanLiteralNode* node) = 0;
    virtual void VisitNode(IntegerLiteralNode* node) = 0;
    virtual void VisitNode(FloatLiteralNode* node) = 0;
    virtual void VisitNode(ColourLiteralNode* node) = 0;
    virtual void VisitNode(PadWidthNode* node) = 0;
    virtual void VisitNode(PadHeightNode* node) = 0;
};
class Parser{
public:
    explicit Parser(vector<string> tk, vector<string> words) :  tokens(std::move(tk)), words(std::move(words)) {}
    unique_ptr<ProgramNode> parseProgram();
private:
    vector<string> tokens;
    vector<string> words;
    int currentToken = 0;
    string currentState = tokens[0];
    stack<string> myStack;
    void consumeToken(const string& expectedToken);
    unique_ptr<ProgramNode> parseProgramNode();
    unique_ptr<StatementNode> parseStatementNode();
    unique_ptr<BlockNode> parseBlockNode();
    unique_ptr<FunctionDeclNode> parseFunctionDeclNode();
    unique_ptr<FormalParamsNode> parseFormalParamsNode();
    unique_ptr<WhileStatementNode> parseWhileStatementNode();
    unique_ptr<ForStatementNode> parseForStatementNode();
    unique_ptr<FormalParamNode> parseFormalParamNode();
    unique_ptr<IfStatementNode> parseIfStatementNode();
    unique_ptr<RtrnStatementNode> parseRtrnStatementNode();
    unique_ptr<PixelStatementNode> parsePixelStatementNode();
    unique_ptr<DelayStatementNode> parseDelayStatementNode();
    unique_ptr<PrintStatementNode> parsePrintStatementNode();
    unique_ptr<AssignmentNode> parseAssignmentNode();
    unique_ptr<VariableDeclNode> parseVariableDeclNode();
    unique_ptr<ExpressionNode> parseExpressionNode();
    unique_ptr<SimpleExpressionNode> parseSimpleExpressionNode();
    unique_ptr<TermNode> parseTermNode();
    unique_ptr<FactorNode> parseFactorNode();
    unique_ptr<UnaryNode> parseUnaryNode();
    unique_ptr<SubExpressionNode> parseSubExpressionNode();
    unique_ptr<FunctionCallNode> parseFunctionCallNode();
    unique_ptr<ActualParamsNode> parseActualParamsNode();
    unique_ptr<PadReadNode> parsePadReadNode();
    unique_ptr<PadRandINode> parsePadRandINode();
    unique_ptr<LiteralNode> parseLiteralNode();
    unique_ptr<IdentifierNode> parseIdentifierNode();
    unique_ptr<TypeNode> parseTypeNode();
    unique_ptr<RelationalOpNode> parseRelationalOpNode();
    unique_ptr<AdditiveOpNode> parseAdditiveOpNode();
    unique_ptr<MultiplicativeOpNode> parseMultiplicativeOpNode();
    unique_ptr<BooleanLiteralNode> parseBooleanLiteralNode();
    unique_ptr<IntegerLiteralNode> parseIntegerLiteralNode();
    unique_ptr<FloatLiteralNode> parseFloatLiteralNode();
    unique_ptr<ColourLiteralNode> parseColourLiteralNode();
    unique_ptr<PadWidthNode> parsePadWidthNode();
    unique_ptr<PadHeightNode> parsePadHeightNode();
};

#endif //UNTITLED_PARSER_H
