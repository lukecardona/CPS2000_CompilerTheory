#include <unordered_map>
#include "parser.h"

#define FUNCTION_TYPE "<Function>"

class SymbolTable {
public:
    void enterScope() {
        symbolTable.emplace_back();
    }

    void exitScope() {
        symbolTable.pop_back();
    }

    bool isDefinedInCurrentScope(const string &name) const {
        return symbolTable.back().find(name) != symbolTable.back().end();
    }

    void define(const string &name, const string &type) {
        symbolTable.back()[name] = type;
    }

    string lookup(const std::string &name) const {
        for (const auto &scope: symbolTable) {
            auto it = scope.find(name);
            if (it != scope.end()) {
                return it->second;
            }
        }
        return "";
    }

    string lastExpressionType;
private:
    vector<unordered_map<string,string>> symbolTable;

};

class SemaniticVisitor : public Visitor{
public:
    void VisitNode(ASTNode* node) override {
        node->programNode->accept(this);
    }
    void VisitNode(ProgramNode* node) override {
        for (const unique_ptr<StatementNode>& statement : node->statements) {
            symbolTable.enterScope();
            statement->accept(this);
            symbolTable.exitScope();
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
        string varName = node->identifier->value;
        string type = "<Function type= '"+node->type->value+"'>";

        //Check that the variable is not already declared
        if(symbolTable.lookup(varName).empty()){
            //Not Found -> Add declaration
            symbolTable.define(varName,type);
        }else{
            //Found
            throw std::runtime_error("\033[1;31m Variable '" + varName + "' already declared \033[0m");
        }

        symbolTable.enterScope();

        if(node->formalParams){
            node->formalParams->accept(this);
        }

        node->block->accept(this);
        if(symbolTable.lastExpressionType != type){
            throw std::runtime_error("\033[1;31m Return Type '" + symbolTable.lastExpressionType + "' is of invalid type \033[0m");
        }


        symbolTable.exitScope();
    }
    void VisitNode(FormalParamsNode* node) override {
        node->formalParamLeft->accept(this);
        for (const unique_ptr<FormalParamNode>& formalParam : node->formalParamRight) {
            formalParam->accept(this);
        }
    }
    void VisitNode(FormalParamNode* node) override {
        string varName = node->identifier->value;
        string type = node->type->value;

        //Check that the variable is not already declared
        if(!symbolTable.isDefinedInCurrentScope(varName)){
            //Not Found -> Add declaration
            symbolTable.define(varName,type);
        }else{
            //Found
            throw std::runtime_error("\033[1;31m Parameter '" + varName + "' already declared \033[0m");
        }
    }
    void VisitNode(WhileStatementNode* node) override {
        symbolTable.enterScope(); //Testing Placement
        node->expression->accept(this);
        if(symbolTable.lastExpressionType != BOOLEAN_LITERAL){
            throw std::runtime_error("\033[1;31m While Statement Expression must be a boolean literal not '" + symbolTable.lastExpressionType +"' \033[0m");
        }
        node->block->accept(this);
        symbolTable.exitScope();
    }
    void VisitNode(ForStatementNode* node) override {
        symbolTable.enterScope(); //Testing Placement
        if(node->varDecl){
            node->varDecl->accept(this);
        }
        node->expression->accept(this);
        if(node->assignment){
            node->block->accept(this);
        }
        node->block->accept(this);
        symbolTable.exitScope();
    }
    void VisitNode(IfStatementNode* node) override {
        node->expression->accept(this);
        if(symbolTable.lastExpressionType != BOOLEAN_LITERAL){
            throw std::runtime_error("\033[1;31m If Statement Expression must be a boolean literal not '" + symbolTable.lastExpressionType +"' \033[0m");
        }
        symbolTable.enterScope(); //Testing Placement
        node->ifBlock->accept(this);
        symbolTable.exitScope();
        if(node->elseBlock) {
            symbolTable.enterScope(); //Testing Placement
            node->elseBlock->accept(this);
            symbolTable.exitScope();
        }
    }
    void VisitNode(RtrnStatementNode* node) override {
        node->expression->accept(this);
    }
    void VisitNode(PixelStatementNode* node) override {
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
    }
    void VisitNode(DelayStatementNode* node) override {
        node->expression->accept(this);
    }
    void VisitNode(PrintStatementNode* node) override {
        node->expression->accept(this);
    }
    void VisitNode(AssignmentNode* node) override {
        string varName = node->identifier->value;
        if(symbolTable.isDefinedInCurrentScope(varName)){
            //Found -> Good
        }else{
            //Not Found
            throw std::runtime_error("\033[1;31m Variable '" + varName + "' not declared \033[0m");
        }
        node->expression->accept(this);
        if(symbolTable.lookup(varName) != symbolTable.lastExpressionType){
            throw std::runtime_error("\033[1;31m Trying to Assign '" + symbolTable.lastExpressionType + "' to '" + symbolTable.lookup(varName)+"' \033[0m");
        }
    }
    void VisitNode(VariableDeclNode* node) override {
        string varName = node->identifier->value;
        string typeName = node->type->value;

        if(symbolTable.isDefinedInCurrentScope(varName)){
            //Found -> Already Declared
            throw std::runtime_error("\033[1;31m Variable '" + varName + "' already declared \033[0m");
        }else{
            //Not Found
            symbolTable.define(varName,typeName);
        }

        node->expression->accept(this);
        if(typeName != symbolTable.lastExpressionType){
            throw std::runtime_error("\033[1;31m Trying to Assign '" + symbolTable.lastExpressionType + "' to '" + symbolTable.lookup(varName)+"' \033[0m");
        }
        cout << symbolTable.lastExpressionType;
    }
    void VisitNode(ExpressionNode* node) override {
        node->leftSimpleExpression->accept(this);;
        int sizeOfRight = node->rightSimpleExpression.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightSimpleExpression[counter])->accept(this);
            get<1>(node->rightSimpleExpression[counter])->accept(this);
            counter++;
            symbolTable.lastExpressionType = "bool";
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
            symbolTable.lastExpressionType = "float";
        }
    }
    void VisitNode(FactorNode* node) override {
        node->accept(this);
    }
    void VisitNode(UnaryNode* node) override {
        node->expression->accept(this);
    }
    void VisitNode(SubExpressionNode* node) override {
        node->expression->accept(this);
    }
    void VisitNode(FunctionCallNode* node) override {
        string varName = node->identifier->value;
        if(symbolTable.lookup(varName).empty()){
            //Not Found -> Throw Error
            throw std::runtime_error("\033[1;31m Variable '" + varName + "' not declared \033[0m");
        }

        if(node->actualParams){
            node->actualParams->accept(this);
        }
    }
    void VisitNode(ActualParamsNode* node) override {
        node->leftExpression->accept(this);
        for (const unique_ptr<ExpressionNode>& expression : node->rightExpressions) {
            expression->accept(this);
        }
    }
    void VisitNode(PadRandINode* node) override {
        node->expression->accept(this);
    }
    void VisitNode(PadReadNode* node) override {
        get<0>(node->expressions)->accept(this);
        get<1>(node->expressions)->accept(this);
    }
    void VisitNode(LiteralNode* node) override {
        node->accept(this);
    }
    void VisitNode(IdentifierNode* node) override {}
    void VisitNode(TypeNode* node) override {}
    void VisitNode(RelationalOpNode* node) override {}
    void VisitNode(AdditiveOpNode* node) override {}
    void VisitNode(MultiplicativeOpNode* node) override {}
    void VisitNode(BooleanLiteralNode* node) override {
        symbolTable.lastExpressionType = "bool";
    }
    void VisitNode(IntegerLiteralNode* node) override {
        symbolTable.lastExpressionType = "int";
    }
    void VisitNode(FloatLiteralNode* node) override {
        symbolTable.lastExpressionType = "float";
    }
    void VisitNode(ColourLiteralNode* node) override {
        symbolTable.lastExpressionType = "color";
    }
    void VisitNode(PadWidthNode* node) override {
        symbolTable.lastExpressionType = PAD_WIDTH;
    }
    void VisitNode(PadHeightNode* node) override {
        symbolTable.lastExpressionType = PAD_HEIGHT;
    }

private:
    SymbolTable symbolTable;
};

/*
int main(){
    //string testString = "let x : int = ( ( 3 * 4 ) + 5 * 7 + ( 0 ) / 100 ) ;";
    string testString = "let c : colour = g ( ) ; fun g ( ) -> int { return 1 ;  }";
    cout << "Testing: " << testString << endl;
    vector<string> splitString = split(testString,' ');
    map<string,map<string,string>> lexerTable = loadMap(); //Loads the map from the CSV file
    map<string, string> finalStates = loadFinalStates();
    vector<string> allLetters = getListOfTokens(testString);
    vector<string> allWords = split(testString,' ');
    vector<string> allTokens = traverseDFSA(lexerTable,finalStates,allLetters);
    vector<string> revisedTokens = reviseTokens(allTokens,allWords);

*/
/*    for (const std::string& str : allTokens) {
        std::cout << str << " ";
    }
    cout << endl;

    for (const std::string& str : revisedTokens) {
        std::cout << str << " ";
    }
    cout << endl;*//*

    system("Color 09");
    revisedTokens.emplace_back(END_OF_FILE);
    unique_ptr<Parser> parser = make_unique<Parser>(revisedTokens,splitString);
    unique_ptr<ProgramNode> tokens = parser->parseProgram();
    unique_ptr<SemaniticVisitor> semaniticVisitor = make_unique<SemaniticVisitor>();
    auto node = new SemaniticVisitor();
    tokens->accept(node);
    cout << endl;
    return 0;
}*/
