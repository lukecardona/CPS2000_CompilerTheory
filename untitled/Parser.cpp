#include <utility>
#include "parser.h"

// Get node type
const string &ASTNode::getNodeType() const {
    return nodeType;
}
const string& StatementNode::getNodeType() const{
    return nodeType;
}
const string& FactorNode::getNodeType() const{
    return nodeType;
}

//AcceptMethods
void ASTNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void ProgramNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void StatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void BlockNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void FunctionDeclNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void FormalParamsNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void FormalParamNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void WhileStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void ForStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void IfStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void RtrnStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void PixelStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void DelayStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void PrintStatementNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void AssignmentNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void VariableDeclNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void ExpressionNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void SimpleExpressionNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void TermNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void FactorNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void UnaryNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void SubExpressionNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void FunctionCallNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void ActualParamsNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void PadRandINode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void PadReadNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void LiteralNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void IdentifierNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void TypeNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void RelationalOpNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void AdditiveOpNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void MultiplicativeOpNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void BooleanLiteralNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void IntegerLiteralNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void FloatLiteralNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void ColourLiteralNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void PadWidthNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
}

void PadHeightNode::accept(Visitor* visitor) {
    visitor->VisitNode(this);
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
unique_ptr<ProgramNode> Parser::parseProgram() {
    unique_ptr<ProgramNode> program = std::move(parseProgramNode());
    return std::move(program);
}
unique_ptr<ProgramNode> Parser::parseProgramNode() {
    unique_ptr<ProgramNode> programNode = make_unique<ProgramNode>();
    while(currentState != END_OF_FILE){
        unique_ptr<StatementNode> statement = parseStatementNode();
        if ((statement->getNodeType() != NULL_PRODUCTION) && (statement != nullptr)) {
            programNode->statements.push_back(std::move(statement));
        }
    }
    return std::move(programNode);
}
unique_ptr<StatementNode> Parser::parseStatementNode(){
    unique_ptr<StatementNode> statementNode = make_unique<StatementNode>();
    //Handle Block
    if (currentState == OPEN_CURLY_BRACKET_TOKEN){
        unique_ptr<BlockNode> block = parseBlockNode();
        if (block) {
            statementNode = std::move(block);
        } //If Block Returned was not empty
        return  std::move(statementNode);
    } //Current State

    //Handle FunctionDecl
    if (currentState == FUN_TOKEN){
        unique_ptr<FunctionDeclNode> functionDecl = parseFunctionDeclNode();
        statementNode = std::move(functionDecl);
        return std::move(statementNode);
    }
    //HandLe Return Statement
    if (currentState == RETURN_TOKEN){
        unique_ptr<RtrnStatementNode> returnStatement = parseRtrnStatementNode();
        statementNode = (std::move(returnStatement));
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }
    //Handle While Statement
    if (currentState == WHILE_TOKEN){
        unique_ptr<WhileStatementNode> whileStatement = parseWhileStatementNode();
        statementNode = std::move(whileStatement);
        return std::move(statementNode);
    }

    //Handle For Statement
    if (currentState == FOR_TOKEN){
        unique_ptr<ForStatementNode> forStatement = parseForStatementNode();
        statementNode = std::move(forStatement);
        return std::move(statementNode);
    }

    //Handle If Statement
    if (currentState == IF_TOKEN){
        unique_ptr<IfStatementNode> ifStatement = parseIfStatementNode();
        statementNode = std::move(ifStatement);
        return std::move(statementNode);
    }

    //Handle Pixel Statement
    if ((currentState == SPECIAL_PIXELR) || (currentState == SPECIAL_PIXEL)){
        unique_ptr<PixelStatementNode> pixelStatement = parsePixelStatementNode();
        statementNode = std::move(pixelStatement);
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    //Handle Delay Statement
    if (currentState == SPECIAL_DELAY){
        unique_ptr<DelayStatementNode> delayStatement = parseDelayStatementNode();
        statementNode = std::move(delayStatement);
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }
    //Handle Print Statement
    if (currentState == SPECIAL_PRINT){
        unique_ptr<PrintStatementNode> printStatement = parsePrintStatementNode();
        statementNode = std::move(printStatement);
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    //Handle Assignment
    if (currentState == IDENTIFIER){
        unique_ptr<AssignmentNode> assignment = parseAssignmentNode();
        statementNode = std::move(assignment);
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    //Handle Variable Declaration
    if (currentState == LET_TOKEN){
        unique_ptr<VariableDeclNode> variableDeclaration = parseVariableDeclNode();
        statementNode = std::move(variableDeclaration);
        consumeToken(SEMICOLON_TOKEN);
        //statementNode->addChild(make_unique<ASTNode>(SEMICOLON_TOKEN));
        return std::move(statementNode);
    }

    unique_ptr<StatementNode> myPtr(nullptr);
    return std::move(myPtr);
}
unique_ptr<BlockNode> Parser::parseBlockNode(){
    unique_ptr<BlockNode> block = make_unique<BlockNode>();
    consumeToken(OPEN_CURLY_BRACKET_TOKEN); //Expect Open Brackets
    while (currentState != CLOSE_CURLY_BRACKET_TOKEN) { //Handle One or multiple statements
        block->statements.push_back(std::move(parseStatementNode()));
    }
    consumeToken(CLOSE_CURLY_BRACKET_TOKEN);
    return std::move(block);
}
unique_ptr<FunctionDeclNode> Parser::parseFunctionDeclNode(){
    unique_ptr<FunctionDeclNode> functionDeclNode = make_unique<FunctionDeclNode>();

    consumeToken(FUN_TOKEN);

    functionDeclNode->identifier = std::move(parseIdentifierNode());

    consumeToken(OPEN_BRACKET_TOKEN);

    if(currentState == IDENTIFIER){
        functionDeclNode->formalParams = std::move(parseFormalParamsNode());
    }

    consumeToken(CLOSE_BRACKET_TOKEN);
    consumeToken("->");

    functionDeclNode->type = std::move(parseTypeNode());

    functionDeclNode->block = std::move(parseBlockNode());

    return std::move(functionDeclNode);
}
unique_ptr<FormalParamsNode> Parser::parseFormalParamsNode(){
    unique_ptr<FormalParamsNode> formalParamsNode = make_unique<FormalParamsNode>();
    formalParamsNode->formalParamLeft = std::move(parseFormalParamNode());
    while (currentState == COMMA_TOKEN){
        consumeToken(COMMA_TOKEN);
        formalParamsNode->formalParamRight.push_back(std::move(parseFormalParamNode()));
    }
    return std::move(formalParamsNode);
}
unique_ptr<FormalParamNode> Parser::parseFormalParamNode(){
    unique_ptr<FormalParamNode> formalParamNode = make_unique<FormalParamNode>();
    formalParamNode->identifier = parseIdentifierNode();
    formalParamNode->type = parseTypeNode();
    return std::move(formalParamNode);
}
unique_ptr<WhileStatementNode> Parser::parseWhileStatementNode(){
    unique_ptr<WhileStatementNode> whileStatementNode = make_unique<WhileStatementNode>();
    consumeToken(WHILE_TOKEN);
    consumeToken(OPEN_BRACKET_TOKEN);
    whileStatementNode->expression = std::move(parseExpressionNode());
    consumeToken(CLOSE_BRACKET_TOKEN);
    whileStatementNode->block = std::move(parseBlockNode());
    return std::move(whileStatementNode);
}
unique_ptr<ForStatementNode> Parser::parseForStatementNode(){
    unique_ptr<ForStatementNode> forStatementNode = make_unique<ForStatementNode>();
    consumeToken(FOR_TOKEN);
    consumeToken(OPEN_BRACKET_TOKEN);
    if(currentState == LET_TOKEN){
        forStatementNode->varDecl = std::move(parseVariableDeclNode());
    }

    consumeToken(SEMICOLON_TOKEN);

    forStatementNode->expression = std::move(parseExpressionNode());

    consumeToken(SEMICOLON_TOKEN);

    if(currentState == IDENTIFIER){
        forStatementNode->assignment = std::move(parseAssignmentNode());
    }

    consumeToken(CLOSE_BRACKET_TOKEN);

    forStatementNode->block = std::move(parseBlockNode());

    return std::move(forStatementNode);
}
unique_ptr<IfStatementNode> Parser::parseIfStatementNode(){
    unique_ptr<IfStatementNode> ifStatementNode = make_unique<IfStatementNode>();

    consumeToken(IF_TOKEN);

    consumeToken(OPEN_BRACKET_TOKEN);

    ifStatementNode->expression = std::move(parseExpressionNode());

    consumeToken(CLOSE_BRACKET_TOKEN);

    if(currentState == ELSE_TOKEN){

        consumeToken(ELSE_TOKEN);
        ifStatementNode->elseBlock = std::move(parseBlockNode());

        return std::move(ifStatementNode);
    }else{
        return std::move(ifStatementNode);
    }
}
unique_ptr<RtrnStatementNode> Parser::parseRtrnStatementNode(){
    unique_ptr<RtrnStatementNode> returnStatementNode = make_unique<RtrnStatementNode>();

    consumeToken(RETURN_TOKEN);
    returnStatementNode->expression = std::move(parseExpressionNode());

    return std::move(returnStatementNode);
}
unique_ptr<PixelStatementNode> Parser::parsePixelStatementNode(){
    unique_ptr<PixelStatementNode> pixelStatementNode = make_unique<PixelStatementNode>();

    if(currentState == SPECIAL_PIXELR) {
        consumeToken(SPECIAL_PIXELR);

        get<0>(pixelStatementNode->pixel_r) = std::move(parseExpressionNode());
        consumeToken(COMMA_TOKEN);

        get<1>(pixelStatementNode->pixel_r) = std::move(parseExpressionNode());
        consumeToken(COMMA_TOKEN);

        get<2>(pixelStatementNode->pixel_r) = std::move(parseExpressionNode());
        consumeToken(COMMA_TOKEN);

        get<3>(pixelStatementNode->pixel_r) = std::move(parseExpressionNode());
        consumeToken(COMMA_TOKEN);

        get<4>(pixelStatementNode->pixel_r) = std::move(parseExpressionNode());
    }

    if(currentState == SPECIAL_PIXEL){
        get<0>(pixelStatementNode->pixel) = std::move(parseExpressionNode());
        consumeToken(COMMA_TOKEN);

        get<1>(pixelStatementNode->pixel) = std::move(parseExpressionNode());
        consumeToken(COMMA_TOKEN);

        get<2>(pixelStatementNode->pixel) = std::move(parseExpressionNode());
    }

    return std::move(pixelStatementNode);
}
unique_ptr<DelayStatementNode> Parser::parseDelayStatementNode(){
    unique_ptr<DelayStatementNode> delayStatementNode = make_unique<DelayStatementNode>();

    consumeToken(SPECIAL_DELAY);

    delayStatementNode->expression = std::move(parseExpressionNode());
    return std::move(delayStatementNode);
}
unique_ptr<PrintStatementNode> Parser::parsePrintStatementNode(){
    unique_ptr<PrintStatementNode> printStatementNode = make_unique<PrintStatementNode>();

    consumeToken(SPECIAL_PRINT);

    printStatementNode->expression = std::move(parseExpressionNode());
    return std::move(printStatementNode);
}
unique_ptr<AssignmentNode> Parser::parseAssignmentNode(){
    unique_ptr<AssignmentNode> assignmentNode = make_unique<AssignmentNode>();

    assignmentNode->identifier = std::move(parseIdentifierNode());

    consumeToken(EQUALS_TOKEN);

    assignmentNode->expression = std::move(parseExpressionNode());

    return std::move(assignmentNode);
}
unique_ptr<VariableDeclNode> Parser::parseVariableDeclNode(){
    unique_ptr<VariableDeclNode> variableDeclNode = make_unique<VariableDeclNode>();

    consumeToken(LET_TOKEN);
    //variableDeclNode->addChild(make_unique<ASTNode>(LET_TOKEN));

    variableDeclNode->identifier = std::move(parseIdentifierNode());

    consumeToken(COLON_TOKEN);

    variableDeclNode->type = std::move(parseTypeNode());

    consumeToken(EQUALS_TOKEN);

    variableDeclNode->expression = std::move(parseExpressionNode());

    return std::move(variableDeclNode);
}
unique_ptr<ExpressionNode> Parser::parseExpressionNode(){
    unique_ptr<ExpressionNode> expressionNode = make_unique<ExpressionNode>();

    expressionNode->leftSimpleExpression = std::move(parseSimpleExpressionNode());

    while(currentState == RELATIONAL_OP){
        unique_ptr<RelationalOpNode> relOp = std::move(parseRelationalOpNode());
        unique_ptr<SimpleExpressionNode> simp = std::move(parseSimpleExpressionNode());
        expressionNode->rightSimpleExpression.emplace_back(std::move(relOp),
                                                           std::move(simp));
    }
    return std::move(expressionNode);
}
unique_ptr<SimpleExpressionNode> Parser::parseSimpleExpressionNode(){
    unique_ptr<SimpleExpressionNode> simpleExpressionNode = make_unique<SimpleExpressionNode>();

    simpleExpressionNode->leftTerm = std::move(parseTermNode());

    while(currentState == ADDITIVE_OP){
        unique_ptr<AdditiveOpNode> additiveOp= std::move(parseAdditiveOpNode());
        unique_ptr<TermNode> termNode = std::move(parseTermNode());
        simpleExpressionNode->rightTerm.emplace_back(std::move(additiveOp),
                                                     std::move(termNode));
    }
    return std::move(simpleExpressionNode);
}
unique_ptr<TermNode> Parser::parseTermNode(){
    unique_ptr<TermNode> termNode = make_unique<TermNode>();

    termNode->leftFactor = std::move(parseFactorNode());

    while(currentState == MULTIPLICATIVE_OP){
        unique_ptr<MultiplicativeOpNode> multOp = std::move(parseMultiplicativeOpNode());
        unique_ptr<FactorNode> factorNode = std::move(parseFactorNode());
        termNode->rightFactor.emplace_back(std::move(multOp),
                                                     std::move(factorNode));
    }
    return std::move(termNode);
}
unique_ptr<FactorNode> Parser::parseFactorNode(){
    unique_ptr<FactorNode> factorNode = make_unique<FactorNode>();

    if ((currentState == IDENTIFIER) && (tokens[currentToken+1] != OPEN_BRACKET_TOKEN)){
        factorNode = std::move(parseIdentifierNode());
        return std::move(factorNode);
    }

    if((currentState == PAD_WIDTH) || (currentState == PAD_HEIGHT)){

    }

    if ((currentState == ADDITIVE_OP) || (currentState == "not")){
        factorNode = std::move(parseUnaryNode());
        return std::move(factorNode);
    }

    if ((currentState == IDENTIFIER)){
        factorNode = std::move(parseFunctionCallNode());
        return std::move(factorNode);
    }

    if((currentState == OPEN_BRACKET_TOKEN)){
        factorNode = std::move(parseSubExpressionNode());
        return std::move(factorNode);
    }

    if(currentState == SPECIAL_RANDI){
        factorNode = std::move(parsePadRandINode());
        return std::move(factorNode);
    }

    if(currentState == SPECIAL_READ){
        factorNode = std::move(parsePadReadNode());
        return std::move(factorNode);
    }

    //Deal With Literals
    if ((currentState == BOOLEAN_LITERAL) || (currentState == INTEGER_LITERAL) || (currentState == FLOAT_LITERAL) ||
        (currentState == COLOR_LITERAL) || (currentState == PAD_WIDTH) || (currentState == PAD_HEIGHT) || (currentState == SPECIAL_READ)){
        factorNode = std::move(parseLiteralNode());
        return std::move(factorNode);
    }

    unique_ptr<FactorNode> myPtr(nullptr);
    return std::move(myPtr);
}
unique_ptr<UnaryNode> Parser::parseUnaryNode(){
    unique_ptr<UnaryNode> unaryNode = make_unique<UnaryNode>();

    if ((currentState == ADDITIVE_OP)){
        consumeToken(ADDITIVE_OP);

        unaryNode->expression = std::move(parseExpressionNode());
        return std::move(unaryNode);
    }

    if ((currentState == "not")){
        consumeToken("not");

        unaryNode->expression = std::move(parseExpressionNode());
        return std::move(unaryNode);
    }

    unique_ptr<UnaryNode> myPtr(nullptr);
    return std::move(myPtr); //Should never be here
}
unique_ptr<SubExpressionNode> Parser::parseSubExpressionNode(){
    unique_ptr<SubExpressionNode> subExpressionNode = make_unique<SubExpressionNode>();

    consumeToken(OPEN_BRACKET_TOKEN);

    subExpressionNode->expression = std::move(parseExpressionNode());

    consumeToken(CLOSE_BRACKET_TOKEN);

    return std::move(subExpressionNode);
}
unique_ptr<PadReadNode> Parser::parsePadReadNode(){
    unique_ptr<PadReadNode> padReadNode = make_unique<PadReadNode>();

    consumeToken(SPECIAL_READ);

    get<0>(padReadNode->expressions) = std::move(parseExpressionNode());
    consumeToken(COMMA_TOKEN);

    get<1>(padReadNode->expressions) = std::move(parseExpressionNode());
    return std::move(padReadNode);
}
unique_ptr<PadRandINode> Parser::parsePadRandINode(){
    unique_ptr<PadRandINode> padRandINode = make_unique<PadRandINode>();

    consumeToken(SPECIAL_RANDI);

    padRandINode->expression = std::move(parseExpressionNode());

    return std::move(padRandINode);
}
unique_ptr<LiteralNode> Parser::parseLiteralNode(){
    unique_ptr<LiteralNode> literalNode = make_unique<LiteralNode>();

    if (currentState == BOOLEAN_LITERAL){
        literalNode = std::move(parseBooleanLiteralNode());
        return std::move(literalNode);
    }
    if(currentState == INTEGER_LITERAL){
        literalNode = std::move(parseIntegerLiteralNode());
        return std::move(literalNode);
    }
    if(currentState == FLOAT_LITERAL){
        literalNode = std::move(parseFloatLiteralNode());
        return std::move(literalNode);
    }
    if(currentState == COLOR_LITERAL){
        literalNode = std::move(parseColourLiteralNode());
        return std::move(literalNode);
    }
    if(currentState == PAD_WIDTH){
        literalNode = std::move(parsePadWidthNode());
        return std::move(literalNode);

    }
    if(currentState == PAD_HEIGHT){
        literalNode = std::move(parsePadHeightNode());
        return std::move(literalNode);
    }
    if(currentState == SPECIAL_READ){
        literalNode = std::move(parsePadReadNode());
        return std::move(literalNode);
    }

    cerr << "\033[1;31m Literal Expected, found" << currentState << " instead \033[0m";
    terminate();
}

unique_ptr<ActualParamsNode> Parser::parseActualParamsNode(){
    unique_ptr<ActualParamsNode> actualParamsNode = make_unique<ActualParamsNode>();

    actualParamsNode->leftExpression = std::move(parseExpressionNode());

    while(currentState == COMMA_TOKEN){
        consumeToken(COMMA_TOKEN);
        actualParamsNode->rightExpressions.emplace_back(std::move(parseExpressionNode()));
    }

    return std::move(actualParamsNode);
}
unique_ptr<FunctionCallNode> Parser::parseFunctionCallNode(){
    unique_ptr<FunctionCallNode> functionCallNode = make_unique<FunctionCallNode>();

    functionCallNode->identifier = std::move(parseIdentifierNode());

    consumeToken(OPEN_BRACKET_TOKEN);

    if(tokens[currentToken+1] != CLOSE_BRACKET_TOKEN){
        functionCallNode->actualParams = std::move(parseActualParamsNode());
        if (functionCallNode->actualParams == nullptr){
            cerr << "\033[1;31m PARSING ERROR: Parsing error with Function Call \033[0m";
        }
    }

    consumeToken(CLOSE_BRACKET_TOKEN);
    //functionCallNode->addChild(make_unique<ASTNode>(CLOSE_BRACKET_TOKEN));

    return std::move(functionCallNode);
}


unique_ptr<IdentifierNode> Parser::parseIdentifierNode(){
    unique_ptr<IdentifierNode> identifierNode = make_unique<IdentifierNode>();
    identifierNode->value = words[currentToken];
    consumeToken(IDENTIFIER);
    return std::move(identifierNode);
}
unique_ptr<TypeNode> Parser::parseTypeNode(){
    unique_ptr<TypeNode> node = make_unique<TypeNode>();
    node->value = words[currentToken];
    consumeToken(TYPE);
    return std::move(node);
}
unique_ptr<RelationalOpNode> Parser::parseRelationalOpNode(){
    unique_ptr<RelationalOpNode> node = make_unique<RelationalOpNode>();
    node->value = words[currentToken];
    consumeToken(RELATIONAL_OP);
    return std::move(node);
}
unique_ptr<AdditiveOpNode> Parser::parseAdditiveOpNode(){
    unique_ptr<AdditiveOpNode> node = make_unique<AdditiveOpNode>();
    node->value = words[currentToken];
    consumeToken(ADDITIVE_OP);
    return std::move(node);
}
unique_ptr<MultiplicativeOpNode> Parser::parseMultiplicativeOpNode(){
    unique_ptr<MultiplicativeOpNode> node = make_unique<MultiplicativeOpNode>();
    node->value = words[currentToken];
    consumeToken(MULTIPLICATIVE_OP);
    return std::move(node);
}
unique_ptr<BooleanLiteralNode> Parser::parseBooleanLiteralNode(){
    unique_ptr<BooleanLiteralNode> node = make_unique<BooleanLiteralNode>();
    node->value = words[currentToken];
    consumeToken(BOOLEAN_LITERAL);
    return std::move(node);
}
unique_ptr<IntegerLiteralNode> Parser::parseIntegerLiteralNode(){
    unique_ptr<IntegerLiteralNode> node = make_unique<IntegerLiteralNode>();
    node->value = words[currentToken];
    consumeToken(INTEGER_LITERAL);
    return std::move(node);
}
unique_ptr<FloatLiteralNode> Parser::parseFloatLiteralNode(){
    unique_ptr<FloatLiteralNode> node = make_unique<FloatLiteralNode>();
    node->value = words[currentToken];
    consumeToken(FLOAT_LITERAL);
    return std::move(node);
}
unique_ptr<ColourLiteralNode> Parser::parseColourLiteralNode(){
    unique_ptr<ColourLiteralNode> node = make_unique<ColourLiteralNode>();
    node->value = words[currentToken];
    consumeToken(COLOR_LITERAL);
    return std::move(node);
}
unique_ptr<PadWidthNode> Parser::parsePadWidthNode(){
    unique_ptr<PadWidthNode> node = make_unique<PadWidthNode>();
    return std::move(node);
}
unique_ptr<PadHeightNode> Parser::parsePadHeightNode(){
    unique_ptr<PadHeightNode> node = make_unique<PadHeightNode>();
    return std::move(node);
}