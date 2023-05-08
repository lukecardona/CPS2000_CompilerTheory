#include "parser.h"

class XMLVisitor : public Visitor {
public:
    void VisitNode(ASTNode* node) override {
       node->programNode->accept(this);
    }
    void VisitNode(ProgramNode* node) override {
        for (const unique_ptr<StatementNode>& statement : node->statements) {
            statement->accept(this);
        }
    }
    void VisitNode(StatementNode* node) override {
        node->accept(this);
    }
    void VisitNode(BlockNode* node) override {
        for (const unique_ptr<StatementNode>& statement : node->statements) {
            statement->accept(this);
        }
    }
    void VisitNode(FunctionDeclNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->identifier->accept(this);
        if(node->formalParams){
            node->formalParams->accept(this);
        }
        node->type->accept(this);
        node->block->accept(this);

        indent_level--;
        tokenNode.insert(1,"\\");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(FormalParamsNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->formalParamLeft->accept(this);;
        for (const unique_ptr<FormalParamNode>& formalParam : node->formalParamRight) {
            formalParam->accept(this);
        }

        indent_level--;
        tokenNode.insert(1,"\\");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(FormalParamNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->identifier->accept(this);
        node->type->accept(this);

        indent_level--;
        tokenNode.insert(1,"\\");
        indent();
        cout << tokenNode <<endl;;
    }
    void VisitNode(WhileStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);
        node->block->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;

    }
    void VisitNode(ForStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        if(node->varDecl){
            node->varDecl->accept(this);
        }
        node->expression->accept(this);
        if(node->assignment){
            node->assignment->accept(this);
        }
        node->block->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(IfStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);
        node->ifBlock->accept(this);
        if(node->elseBlock){
           node->elseBlock->accept(this);
        }

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(RtrnStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
       node->expression->accept(this);;

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(PixelStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;
        if (get<0>(node->pixel_r)){
            get<0>(node->pixel_r)->accept(this);
            get<1>(node->pixel_r)->accept(this);
            get<2>(node->pixel_r)->accept(this);
            get<3>(node->pixel_r)->accept(this);
            get<4>(node->pixel_r)->accept(this);
        }
        if (get<0>(node->pixel)){
            get<0>(node->pixel_r)->accept(this);
            get<1>(node->pixel_r)->accept(this);
            get<2>(node->pixel_r)->accept(this);
        }

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(DelayStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(PrintStatementNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(AssignmentNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode <<endl;
        indent_level++;

        //Change Type of transition for every node
        node->identifier->accept(this);
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(VariableDeclNode* node) override {
        string tokenNode = VARIABLE_DECL;
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        indent();
        indent_level++;
        cout << "<Variable type= '"+node->type->value+"' >" << endl;
        node->identifier->accept(this);
        indent_level--;
        indent();
        cout << "</Variable>" << endl;
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode;
    }
    void VisitNode(ExpressionNode* node) override {
        //Change Type of transition for every node
        node->leftSimpleExpression->accept(this);;
        int sizeOfRight = node->rightSimpleExpression.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightSimpleExpression[counter])->accept(this);
            get<1>(node->rightSimpleExpression[counter])->accept(this);
            counter++;
        }
    }
    void VisitNode(SimpleExpressionNode* node) override {
        //Change Type of transition for every node
        node->leftTerm->accept(this);
        int sizeOfRight = node->rightTerm.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightTerm[counter])->accept(this);
            get<1>(node->rightTerm[counter])->accept(this);
            counter++;
        }

    }
    void VisitNode(TermNode* node) override {
        //Change Type of transition for every node
        node->leftFactor->accept(this);
        int sizeOfRight = node->rightFactor.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightFactor[counter])->accept(this);
            get<1>(node->rightFactor[counter])->accept(this);
            counter++;
        }
    }
    void VisitNode(FactorNode* node) override {
        node->accept(this);
    }
    void VisitNode(UnaryNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode <<endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(SubExpressionNode* node) override {
        string tokenNode = SUB_EXPRESSION;
        indent();
        cout << tokenNode <<endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(FunctionCallNode* node) override {
        string tokenNode = FUNCTION_CALL;
        indent();
        cout << tokenNode <<endl;
        indent_level++;

        //Change Type of transition for every node
        node->identifier->accept(this);
        if(node->actualParams){
            node->actualParams->accept(this);
        }

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(ActualParamsNode* node) override {
        string tokenNode = ACTUAL_PARAMS;
        indent();
        cout << tokenNode <<endl;
        indent_level++;

        //Change Type of transition for every node
        node->leftExpression->accept(this);;
        for (const unique_ptr<ExpressionNode>& expressions : node->rightExpressions) {
            expressions->accept(this);
        }

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(PadRandINode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode <<endl;
        indent_level++;

        //Change Type of transition for every node
        node->expression->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(PadReadNode* node) override {
        string tokenNode = node->getNodeType();
        indent();
        cout << tokenNode << endl;
        indent_level++;

        //Change Type of transition for every node
        get<0>(node->expressions)->accept(this);
        get<1>(node->expressions)->accept(this);

        tokenNode.insert(1,"\\");
        indent_level--;
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(LiteralNode* node) override {
        node->accept(this);
    }
    void VisitNode(IdentifierNode* node) override {
        string tokenNode = IDENTIFIER;
        indent();
        indent_level++;
        cout << tokenNode <<endl;
        indent();
        indent_level--;
        cout << node->value << endl;
        tokenNode.insert(1,"\\");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(TypeNode* node) override {
        string tokenNode = TYPE;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value+"'");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(RelationalOpNode* node) override {
        string tokenNode = RELATIONAL_OP;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1," Op = '"+node->value+"'");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(AdditiveOpNode* node) override {
        string tokenNode = ADDITIVE_OP;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1," Op = '"+node->value+"'");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(MultiplicativeOpNode* node) override {
        string tokenNode = MULTIPLICATIVE_OP;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1," Op = '"+node->value+"'");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(BooleanLiteralNode* node) override {
        string tokenNode = BOOLEAN_LITERAL;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value)+"'";
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(IntegerLiteralNode* node) override {
        string tokenNode = INTEGER_LITERAL;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value+"'");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(FloatLiteralNode* node) override {
        string tokenNode = FLOAT_LITERAL;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value+"'");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(ColourLiteralNode* node) override {
        string tokenNode = COLOR_LITERAL;
        tokenNode.insert(1,"\\");
        tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value);
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(PadWidthNode* node) override {
        string tokenNode = PAD_WIDTH;
        tokenNode.insert(1,"\\");
        indent();
        cout << tokenNode <<endl;
    }
    void VisitNode(PadHeightNode* node) override {
        string tokenNode = PAD_HEIGHT;
        tokenNode.insert(1,"\\");
        indent();
        cout << tokenNode <<endl;
    }
private:
    int indent_level{};

    void indent() const {
        for (int i = 0; i < indent_level * 2; ++i) {
            std::cout << ' ';
        }
    }
};

//int main(){
//    string testString = "let x : int = ( ( 3 * 4 ) + 5 * 7 + ( 0 ) / 100 ) ;";
//    cout << "Testing: " << testString << endl;
//    vector<string> splitString = split(testString,' ');
//    map<string,map<string,string>> lexerTable = loadMap(); //Loads the map from the CSV file
//    map<string, string> finalStates = loadFinalStates();
//    vector<string> allLetters = getListOfTokens(testString);
//    vector<string> allWords = split(testString,' ');
//    vector<string> allTokens = traverseDFSA(lexerTable,finalStates,allLetters);
//    vector<string> revisedTokens = reviseTokens(allTokens,allWords);
//
///*    for (const std::string& str : allTokens) {
//        std::cout << str << " ";
//    }
//    cout << endl;
//
//    for (const std::string& str : revisedTokens) {
//        std::cout << str << " ";
//    }
//    cout << endl;*/
//    system("Color 09");
//    revisedTokens.emplace_back(END_OF_FILE);
//    unique_ptr<Parser> parser = make_unique<Parser>(revisedTokens,splitString);
//    unique_ptr<ProgramNode> tokens = parser->parseProgram();
//    unique_ptr<XMLVisitor> xmlVisitor = make_unique<XMLVisitor>();
//    auto node = new XMLVisitor();
//    tokens->accept(node);
//    cout << endl;
//    return 0;
//}