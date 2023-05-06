#include <utility>
#include "parser.h"

// Get node type
const string &ASTNode::getNodeType() const {
    return nodeType;
}
void ASTNode::addChild(unique_ptr<ASTNode> child) {
    children.push_back(std::move(child));
}
const vector<unique_ptr<ASTNode>> &ASTNode::getChildren() const {
    return children;
}

void Parser::consumeToken(const std::string &expectedToken) {
    if (currentState == expectedToken){
        currentToken += 1;
        currentState = tokens[currentToken];
    }
    else{
        cerr << "\033[1;31m PARSING ERROR: Wrong expected token, expected token: " << expectedToken <<
             "got " << currentState << "Instead \033[0m";
        terminate();
    }
}

unique_ptr<ASTNode> Parser::parseProgram() {
    unique_ptr<ASTNode> children = std::move(parseProgramNode());
    return std::move(children);
}
unique_ptr<ASTNode> Parser::parseProgramNode() {
    unique_ptr<ProgramNode> programNode = make_unique<ProgramNode>();
    while(currentState != END_OF_FILE){
        unique_ptr<ASTNode> statement = parseStatementNode();
        if (statement->getNodeType() != NULL_PRODUCTION) {
            programNode->addChild(std::move(statement));
        }
    }
    return std::move(programNode);
}
unique_ptr<ASTNode> Parser::parseStatementNode(){
    unique_ptr<StatementNode> statementNode = make_unique<StatementNode>();
    //Handle Block
    if (currentState == OPEN_CURLY_BRACKET_TOKEN){
        unique_ptr<ASTNode> block = parseBlockNode();
        if (block) {
            statementNode->addChild(std::move(block));
        } //If Block Returned was not empty
        return  std::move(statementNode);
    } //Current State

    //Handle FunctionDecl
    if (currentState == FUN_TOKEN){
        unique_ptr<ASTNode> functionDecl = parseFunctionDeclNode();
        statementNode->addChild(std::move(functionDecl));
        return std::move(statementNode);
    }
    //HandLe Return Statement
    if (currentState == RETURN_TOKEN){
        unique_ptr<ASTNode> returnStatement = parseRtrnStatementNode();
        statementNode->addChild(std::move(returnStatement));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }
    //Handle While Statement
    if (currentState == WHILE_TOKEN){
        unique_ptr<ASTNode> whileStatement = parseWhileStatementNode();
        statementNode->addChild(std::move(whileStatement));
        return std::move(statementNode);
    }

    //Handle For Statement
    if (currentState == FOR_TOKEN){
        unique_ptr<ASTNode> forStatement = parseForStatementNode();
        statementNode->addChild(std::move(forStatement));
        return std::move(statementNode);
    }

    //Handle If Statement
    if (currentState == IF_TOKEN){
        unique_ptr<ASTNode> ifStatement = parseIfStatementNode();
        statementNode->addChild(std::move(ifStatement));
        return std::move(statementNode);
    }

    //Handle Pixel Statement
    if (currentState == SPECIAL_PIXELR){
        unique_ptr<ASTNode> pixelStatement = parsePixelStatementNode();
        statementNode->addChild(std::move(pixelStatement));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    //Handle Delay Statement
    if (currentState == SPECIAL_DELAY){
        unique_ptr<ASTNode> delayStatement = parseDelayStatementNode();
        statementNode->addChild(std::move(delayStatement));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }
    //Handle Print Statement
    if (currentState == SPECIAL_PRINT){
        unique_ptr<ASTNode> printStatement = parsePrintStatementNode();
        statementNode->addChild(std::move(printStatement));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    //Handle Assignment
    if (currentState == IDENTIFIER){
        unique_ptr<ASTNode> assignment = parseAssignmentNode();
        statementNode->addChild(std::move(assignment));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    //Handle Variable Declaration
    if (currentState == LET_TOKEN){
        unique_ptr<ASTNode> variableDeclaration = parseVariableDeclNode();
        statementNode->addChild(std::move(variableDeclaration));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    return std::move(make_unique<ASTNode>(NULL_PRODUCTION));
}
unique_ptr<ASTNode> Parser::parseBlockNode(){
    unique_ptr<BlockNode> block = make_unique<BlockNode>();
    consumeToken(OPEN_CURLY_BRACKET_TOKEN); //Expect Open Brackets
    while (currentState != CLOSE_CURLY_BRACKET_TOKEN) { //Handle One or multiple statements
        block->addChild(std::move(parseStatementNode()));
    }
    consumeToken(CLOSE_CURLY_BRACKET_TOKEN);
    return std::move(block);
}
unique_ptr<ASTNode> Parser::parseFunctionDeclNode(){
    unique_ptr<FunctionDeclNode> functionDeclNode = make_unique<FunctionDeclNode>();

    consumeToken(FUN_TOKEN);
    consumeToken(IDENTIFIER);
    consumeToken(OPEN_BRACKET_TOKEN);
    if(currentState == IDENTIFIER){
        functionDeclNode->addChild(std::move(parseFormalParamsNode()));
    }
    consumeToken(CLOSE_BRACKET_TOKEN);
    consumeToken("->");
    consumeToken(TYPE);
    functionDeclNode->addChild(std::move(parseBlockNode()));
    return std::move(functionDeclNode);
}
unique_ptr<ASTNode> Parser::parseFormalParamsNode(){
    unique_ptr<FormalParamsNode> formalParamsNode = make_unique<FormalParamsNode>();
    formalParamsNode->addChild(std::move(parseFormalParamNode()));
    while (currentState == COMMA_TOKEN){
        consumeToken(COMMA_TOKEN);
        formalParamsNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));
        formalParamsNode->addChild(std::move(parseFormalParamNode()));
    }
    return std::move(formalParamsNode);
}
unique_ptr<ASTNode> Parser::parseFormalParamNode(){
    unique_ptr<FormalParamNode> formalParamNode = make_unique<FormalParamNode>();
    return std::move(formalParamNode);
}
unique_ptr<ASTNode> Parser::parseWhileStatementNode(){
    unique_ptr<WhileStatementNode> whileStatementNode = make_unique<WhileStatementNode>();
    consumeToken(WHILE_TOKEN);
    whileStatementNode->addChild(make_unique<ASTNode>(WHILE_TOKEN));
    consumeToken(OPEN_BRACKET_TOKEN);
    whileStatementNode->addChild(make_unique<ASTNode>(OPEN_BRACKET_TOKEN));
    whileStatementNode->addChild(std::move(parseExpressionNode()));
    consumeToken(CLOSE_BRACKET_TOKEN);
    whileStatementNode->addChild(make_unique<ASTNode>(CLOSE_BRACKET_TOKEN));
    whileStatementNode->addChild(std::move(parseBlockNode()));
    return std::move(whileStatementNode);
}
unique_ptr<ASTNode> Parser::parseForStatementNode(){
    unique_ptr<ForStatementNode> forStatementNode = make_unique<ForStatementNode>();
    consumeToken(FOR_TOKEN);
    forStatementNode->addChild(make_unique<ASTNode>(FOR_TOKEN));

    consumeToken(OPEN_BRACKET_TOKEN);
    forStatementNode->addChild(make_unique<ASTNode>(OPEN_BRACKET_TOKEN));

    if(currentState == LET_TOKEN){
        forStatementNode->addChild(std::move(parseVariableDeclNode()));
    }

    consumeToken(SEMICOLON_TOKEN);
    forStatementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));

    forStatementNode->addChild(std::move(parseExpressionNode()));

    consumeToken(SEMICOLON_TOKEN);
    forStatementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));

    if(currentState == IDENTIFIER){
        forStatementNode->addChild(std::move(parseAssignmentNode()));
    }

    consumeToken(CLOSE_BRACKET_TOKEN);
    forStatementNode->addChild(make_unique<ASTNode>(CLOSE_BRACKET_TOKEN));

    forStatementNode->addChild(std::move(parseBlockNode()));

    return std::move(forStatementNode);
}
unique_ptr<ASTNode> Parser::parseIfStatementNode(){
    unique_ptr<IfStatementNode> ifStatementNode = make_unique<IfStatementNode>();

    consumeToken(IF_TOKEN);
    ifStatementNode->addChild(make_unique<ASTNode>(IF_TOKEN));

    consumeToken(OPEN_BRACKET_TOKEN);
    ifStatementNode->addChild(make_unique<ASTNode>(OPEN_BRACKET_TOKEN));

    ifStatementNode->addChild(std::move(parseExpressionNode()));

    consumeToken(CLOSE_BRACKET_TOKEN);
    ifStatementNode->addChild(make_unique<ASTNode>(CLOSE_BRACKET_TOKEN));

    if(currentState == ELSE_TOKEN){

        consumeToken(ELSE_TOKEN);
        ifStatementNode->addChild(make_unique<ASTNode>(ELSE_TOKEN));

        ifStatementNode->addChild(std::move(parseBlockNode()));
        return std::move(ifStatementNode);
    }else{
        return std::move(ifStatementNode);
    }
}
unique_ptr<ASTNode> Parser::parseRtrnStatementNode(){
    unique_ptr<RtrnStatementNode> returnStatementNode = make_unique<RtrnStatementNode>();

    consumeToken(RETURN_TOKEN);
    returnStatementNode->addChild(make_unique<ASTNode>(RETURN_TOKEN));

    returnStatementNode->addChild(std::move(parseExpressionNode()));

    return std::move(returnStatementNode);
}
unique_ptr<ASTNode> Parser::parsePixelStatementNode(){
    unique_ptr<PixelStatementNode> pixelStatementNode = make_unique<PixelStatementNode>();

    consumeToken(SPECIAL_PIXELR);
    pixelStatementNode->addChild(make_unique<ASTNode>(SPECIAL_PIXELR));

    pixelStatementNode->addChild(std::move(parseExpressionNode()));
    consumeToken(COMMA_TOKEN);
    pixelStatementNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));

    pixelStatementNode->addChild(std::move(parseExpressionNode()));
    consumeToken(COMMA_TOKEN);
    pixelStatementNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));

    pixelStatementNode->addChild(std::move(parseExpressionNode()));
    consumeToken(COMMA_TOKEN);
    pixelStatementNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));

    pixelStatementNode->addChild(std::move(parseExpressionNode()));
    consumeToken(COMMA_TOKEN);
    pixelStatementNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));

    pixelStatementNode->addChild(std::move(parseExpressionNode()));

    return std::move(pixelStatementNode);
}
unique_ptr<ASTNode> Parser::parseDelayStatementNode(){
    unique_ptr<DelayStatementNode> delayStatementNode = make_unique<DelayStatementNode>();

    consumeToken(SPECIAL_DELAY);
    delayStatementNode->addChild(make_unique<ASTNode>(SPECIAL_DELAY));

    delayStatementNode->addChild(std::move(parseExpressionNode()));
    return std::move(delayStatementNode);
}
unique_ptr<ASTNode> Parser::parsePrintStatementNode(){
    unique_ptr<PrintStatementNode> printStatementNode = make_unique<PrintStatementNode>();

    consumeToken(SPECIAL_PRINT);
    printStatementNode->addChild(make_unique<ASTNode>(SPECIAL_PRINT));

    printStatementNode->addChild(std::move(parseExpressionNode()));
    return std::move(printStatementNode);
}
unique_ptr<ASTNode> Parser::parseAssignmentNode(){
    unique_ptr<AssignmentNode> assignmentNode = make_unique<AssignmentNode>();

    consumeToken(IDENTIFIER);
    assignmentNode->addChild(make_unique<ASTNode>(IDENTIFIER));

    consumeToken(EQUALS_TOKEN);
    assignmentNode->addChild(make_unique<ASTNode>(EQUALS_TOKEN));

    assignmentNode->addChild(std::move(parseExpressionNode()));

    return std::move(assignmentNode);
}
unique_ptr<ASTNode> Parser::parseVariableDeclNode(){
    unique_ptr<VariableDeclNode> variableDeclNode = make_unique<VariableDeclNode>();

    consumeToken(LET_TOKEN);
    //variableDeclNode->addChild(make_unique<ASTNode>(LET_TOKEN));

    consumeToken(IDENTIFIER);
    variableDeclNode->addChild(make_unique<ASTNode>(IDENTIFIER));

    consumeToken(COLON_TOKEN);
    //variableDeclNode->addChild(make_unique<ASTNode>(COLON_TOKEN));

    consumeToken(TYPE);
    variableDeclNode->addChild(make_unique<ASTNode>(TYPE));

    consumeToken(EQUALS_TOKEN);
    variableDeclNode->addChild(make_unique<ASTNode>(EQUALS_TOKEN));

    variableDeclNode->addChild(std::move(parseExpressionNode()));

    return std::move(variableDeclNode);
}
unique_ptr<ASTNode> Parser::parseExpressionNode(){
    unique_ptr<ExpressionNode> expressionNode = make_unique<ExpressionNode>();

    expressionNode->addChild(std::move(parseSimpleExpressionNode()));

    while(currentState == RELATIONAL_OP){
        consumeToken(RELATIONAL_OP);
        expressionNode->addChild(make_unique<ASTNode>(RELATIONAL_OP));

        expressionNode->addChild(std::move(parseSimpleExpressionNode()));
    }
    return std::move(expressionNode);
}
unique_ptr<ASTNode> Parser::parseSimpleExpressionNode(){
    unique_ptr<SimpleExpressionNode> simpleExpressionNode = make_unique<SimpleExpressionNode>();

    simpleExpressionNode->addChild(std::move(parseTermNode()));

    while(currentState == ADDITIVE_OP){
        consumeToken(ADDITIVE_OP);
        simpleExpressionNode->addChild(make_unique<ASTNode>(ADDITIVE_OP));

        simpleExpressionNode->addChild(std::move(parseTermNode()));
    }
    return std::move(simpleExpressionNode);
}
unique_ptr<ASTNode> Parser::parseTermNode(){
    unique_ptr<TermNode> termNode = make_unique<TermNode>();

    termNode->addChild(std::move(parseFactorNode()));

    while(currentState == MULTIPLICATIVE_OP){
        consumeToken(MULTIPLICATIVE_OP);
        termNode->addChild(make_unique<ASTNode>(MULTIPLICATIVE_OP));

        termNode->addChild(std::move(parseFactorNode()));
    }
    return std::move(termNode);
}
unique_ptr<ASTNode> Parser::parseFactorNode(){
    unique_ptr<FactorNode> factorNode = make_unique<FactorNode>();

    if ((currentState == IDENTIFIER) || (currentState == PAD_WIDTH) || (currentState == PAD_HEIGHT)){
        factorNode->addChild(make_unique<ASTNode>(currentState));
        consumeToken(currentState);
        return std::move(factorNode);
    }

    if ((currentState == ADDITIVE_OP) || (currentState == "not")){
        factorNode->addChild(std::move(parseUnaryNode()));
        return std::move(factorNode);
    }

    if ((currentState == IDENTIFIER)){
        factorNode->addChild(std::move(parseFunctionCallNode()));
        return std::move(factorNode);
    }

    if((currentState == OPEN_BRACKET_TOKEN)){
        factorNode->addChild(std::move(parseSubExpressionNode()));
        return std::move(factorNode);
    }

    if(currentState == SPECIAL_RANDI){
        factorNode->addChild(std::move(parsePadRandINode()));
        return std::move(factorNode);
    }

    if(currentState == SPECIAL_READ){
        factorNode->addChild(std::move(parsePadReadNode()));
        return std::move(factorNode);
    }

    //Deal With Literals
    if ((currentState == BOOLEAN_LITERAL) || (currentState == INTEGER_LITERAL) || (currentState == FLOAT_LITERAL) ||
        (currentState == COLOR_LITERAL) || (currentState == PAD_WIDTH) || (currentState == PAD_HEIGHT) || (currentState == SPECIAL_READ)){
        factorNode->addChild(std::move(parseLiteralNode()));
        return std::move(factorNode);
    }

    return std::move(make_unique<ASTNode>(NULL_PRODUCTION));;
}
unique_ptr<ASTNode> Parser::parseUnaryNode(){
    unique_ptr<UnaryNode> unaryNode = make_unique<UnaryNode>();

    if ((currentState == ADDITIVE_OP)){
        consumeToken(ADDITIVE_OP);
        unaryNode->addChild(make_unique<ASTNode>("-"));

        unaryNode->addChild(std::move(parseExpressionNode()));
        return std::move(unaryNode);
    }

    if ((currentState == "not")){
        consumeToken("not");
        unaryNode->addChild(make_unique<ASTNode>("not"));

        unaryNode->addChild(std::move(parseExpressionNode()));
        return std::move(unaryNode);
    }

    return std::move(make_unique<ASTNode>(NULL_PRODUCTION)); //Should never be here
}
unique_ptr<ASTNode> Parser::parseSubExpressionNode(){
    unique_ptr<SubExpressionNode> subExpressionNode = make_unique<SubExpressionNode>();

    consumeToken(OPEN_BRACKET_TOKEN);
    subExpressionNode->addChild(make_unique<ASTNode>(OPEN_BRACKET_TOKEN));

    subExpressionNode->addChild(std::move(parseExpressionNode()));

    consumeToken(CLOSE_BRACKET_TOKEN);
    subExpressionNode->addChild(make_unique<ASTNode>(CLOSE_BRACKET_TOKEN));

    return std::move(subExpressionNode);
}
unique_ptr<ASTNode> Parser::parsePadReadNode(){
    unique_ptr<PadReadNode> padReadNode = make_unique<PadReadNode>();

    consumeToken(SPECIAL_READ);
    padReadNode->addChild(make_unique<ASTNode>(SPECIAL_READ));

    padReadNode->addChild(std::move(parseExpressionNode()));
    consumeToken(COMMA_TOKEN);
    padReadNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));

    padReadNode->addChild(std::move(parseExpressionNode()));
    return std::move(padReadNode);
}
unique_ptr<ASTNode> Parser::parsePadRandINode(){
    unique_ptr<PadRandINode> padRandINode = make_unique<PadRandINode>();

    consumeToken(SPECIAL_RANDI);
    padRandINode->addChild(make_unique<ASTNode>(SPECIAL_RANDI));

    padRandINode->addChild(std::move(parseExpressionNode()));

    return std::move(padRandINode);
}
unique_ptr<ASTNode> Parser::parseLiteralNode(){
    unique_ptr<LiteralNode> literalNode = make_unique<LiteralNode>();

    if ((currentState == BOOLEAN_LITERAL) || (currentState == INTEGER_LITERAL) || (currentState == FLOAT_LITERAL) ||
        (currentState == COLOR_LITERAL) || (currentState == PAD_WIDTH) || (currentState == PAD_HEIGHT)){
        literalNode->addChild(make_unique<ASTNode>(currentState));
        consumeToken(currentState);
        return std::move(literalNode);
    }

    if(currentState == SPECIAL_READ){
        literalNode->addChild(std::move(parsePadReadNode()));
        return std::move(literalNode);
    }

    cerr << "\033[1;31m Literal Expected, found" << currentState << " instead \033[0m";
    terminate();
    return std::move(make_unique<ASTNode>(NULL_PRODUCTION));
}
unique_ptr<ASTNode> Parser::parseActualParamsNode(){
    unique_ptr<ActualParamsNode> actualParamsNode = make_unique<ActualParamsNode>();

    actualParamsNode->addChild(std::move(parseExpressionNode()));

    if(currentState == COMMA_TOKEN){
        consumeToken(COMMA_TOKEN);
        actualParamsNode->addChild(make_unique<ASTNode>(COMMA_TOKEN));

        actualParamsNode->addChild(std::move(parseExpressionNode()));
    }

    return std::move(actualParamsNode);
}
unique_ptr<ASTNode> Parser::parseFunctionCallNode(){
    unique_ptr<FunctionCallNode> functionCallNode = make_unique<FunctionCallNode>();

    consumeToken(IDENTIFIER);
    functionCallNode->addChild(make_unique<ASTNode>(IDENTIFIER));

    consumeToken(OPEN_BRACKET_TOKEN);
//      functionCallNode->addChild(make_unique<ASTNode>(OPEN_BRACKET_TOKEN));

    unique_ptr<ASTNode> actualParamsNode = std::move(parseActualParamsNode());
    if(actualParamsNode){
        functionCallNode->addChild(std::move(actualParamsNode));
    }

    consumeToken(CLOSE_BRACKET_TOKEN);
    //functionCallNode->addChild(make_unique<ASTNode>(CLOSE_BRACKET_TOKEN));

    return std::move(functionCallNode);
}