#include <unordered_map>
#include "parser.h"

#define OFRAME "oframe"

class codeGenerationHelper{
public:
    //Symbol Table Functions
    void enterScope() {
        symbolTable.emplace_back();
    }
    void exitScope() {
        symbolTable.pop_back();
    }
    void insertSymbol(const string& name) {
        symbolTable.back()[name] = memoryLocations[currentFunction];
        memoryLocations[currentFunction] ++;
    }
    int getSymbolLocation(const std::string& name) {
        for (auto it = symbolTable.rbegin(); it != symbolTable.rend(); ++it) {
            auto& table = *it;
            auto entry = table.find(name);
            if (entry != table.end()) {
                return entry->second;
            }
        }
        throw runtime_error("\033[1;31m Code Generation Error: '" + name + "' Instruction was not found \033[0m");
    }

    int addInstruction(const string& inst){
        instructions[currentFunction].push_back(inst);
        currentInstructionIndex++;
        return currentInstructionIndex;
    }

    int addInstruction(const string& inst1, const string& inst2){
        instructions[currentFunction].push_back(inst1+" "+inst2);
        currentInstructionIndex++;
        return currentInstructionIndex;
    }

    void replaceInstruction(int index,const string& inst){
        instructions[currentFunction][index] = inst;
        currentInstructionIndex++;
    }

    void replaceInstruction(int index,const string& inst,const string& inst2){
        instructions[currentFunction][index] = inst+" "+inst2;
        currentInstructionIndex++;
    }

    void startFunction(const string& name){
        enterScope();
        currentInstructionIndex = 0;
        maximumFunctions++;
        currentFunction = maximumFunctions;
        std::vector<string> newVec;
        instructions.push_back(newVec);
        memoryLocations.push_back(0);
        addInstruction("."+name);
    }

    void endFunction(){
        currentInstructionIndex = instructions[0].size()-1;
        exitScope();
    }

    int getCurrentInstructionIndex(){
        return currentInstructionIndex;
    };

    void compileInstructions(){
        for (const auto& instruction_vector : instructions) {
            for (const auto& instruction : instruction_vector) {
                std::cout << instruction << std::endl;
            }
            std::cout << std::endl;
        }
    }

private:
    vector<vector<string>> instructions;
    vector<unordered_map<string, int>> symbolTable;
    vector<int> memoryLocations;
    int currentInstructionIndex = 0;
    int currentFunction = -1;
    int maximumFunctions = -1;
};

class CodeGeneration : public Visitor{
public:
    void VisitNode(ASTNode* node) override {
        node->programNode->accept(this);

    }
    void VisitNode(ProgramNode* node) override {
        CGH->startFunction("main");
        for (const unique_ptr<StatementNode>& statement : node->statements) {
            statement->accept(this);
        }
        CGH->addInstruction("halt");
        CGH->compileInstructions();
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
        CGH->startFunction("."+node->identifier->value);
        if(node->formalParams){
            node->formalParams->accept(this);
        }
        node->block->accept(this);
        CGH->endFunction();
    }
    void VisitNode(FormalParamsNode* node) override {
        node->formalParamLeft->accept(this);
        for (const unique_ptr<FormalParamNode>& statement : node->formalParamRight) {
            statement->accept(this);
        }
    }
    void VisitNode(FormalParamNode* node) override {
        CGH->insertSymbol(node->identifier->value);
    }
    void VisitNode(WhileStatementNode* node) override {
        int loopStartId = CGH->getCurrentInstructionIndex();
        node->expression->accept(this);
        CGH->addInstruction("push", "0");
        CGH->addInstruction("eq");
        int jumpAfterLoopIdx = CGH->addInstruction("cjmp2", "<PLACEHOLDER>");

        node->block->accept(this);

        CGH->addInstruction("jmp", std::to_string(loopStartId));
        CGH->replaceInstruction(jumpAfterLoopIdx, std::to_string(CGH->getCurrentInstructionIndex()));
    }
    void VisitNode(ForStatementNode* node) override {
        if (node->varDecl) {
            node->varDecl->accept(this);
        }
        int loopStartId = CGH->getCurrentInstructionIndex();
        node->expression->accept(this);
        CGH->addInstruction("push", "0");
        CGH->addInstruction("eq");

        int jumpAfterLoopId = CGH->addInstruction("cjmp2", "<PLACEHOLDER>");

        node->block->accept(this);

        if (node->assignment) {
            node->assignment->accept(this);
        }
        CGH->addInstruction("jmp", std::to_string(loopStartId));
        CGH->replaceInstruction(jumpAfterLoopId, to_string(CGH->getCurrentInstructionIndex()));
    }
    void VisitNode(IfStatementNode* node) override {
        node->expression->accept(this);
        CGH->addInstruction("push ", "0");
        CGH->addInstruction("eq");

        int jumpToElseId = CGH->addInstruction("cjmp2", "<PLACEHOLDER>");

        node->ifBlock->accept(this);

        int jumpAfterIfId = -1;
        if (node->elseBlock) {
            jumpAfterIfId = CGH->addInstruction("jmp", "<PLACEHOLDER>");
        }

        CGH->replaceInstruction(jumpToElseId, to_string(CGH->getCurrentInstructionIndex()));

        if (node->elseBlock) {
            node->elseBlock->accept(this);
            CGH->replaceInstruction(jumpAfterIfId, to_string(CGH->getCurrentInstructionIndex()));
        }
    }
    void VisitNode(RtrnStatementNode* node) override {
        node->expression->accept(this);
        CGH->addInstruction("ret");
    }
    void VisitNode(PixelStatementNode* node) override {
        if(get<0>(node->pixel_r)){
            get<0>(node->pixel_r)->accept(this);
            get<1>(node->pixel_r)->accept(this);
            get<2>(node->pixel_r)->accept(this);
            get<3>(node->pixel_r)->accept(this);
            get<4>(node->pixel_r)->accept(this);
            CGH->addInstruction("pixelr");
            return;
        }
        if(get<0>(node->pixel)){
            get<0>(node->pixel)->accept(this);
            get<1>(node->pixel)->accept(this);
            get<2>(node->pixel)->accept(this);
            CGH->addInstruction("pixel");
            return;
        }
        throw runtime_error("\033[1;31m Code Generation Error: Pixel Statement unable to be generated \033[0m");
    }
    void VisitNode(DelayStatementNode* node) override {
        node->expression->accept(this);
        CGH->addInstruction("delay");
    }
    void VisitNode(PrintStatementNode* node) override {
        node->expression->accept(this);
        CGH->addInstruction("print");
    }
    void VisitNode(AssignmentNode* node) override {
        node->expression->accept(this); // evaluate the expression
        int memoryLocation = CGH->getSymbolLocation(node->identifier->value);
        CGH->addInstruction("push", to_string(memoryLocation)); // target location
        CGH->addInstruction("st");
    }
    void VisitNode(VariableDeclNode* node) override {
        node->expression->accept(this);
        CGH->insertSymbol(node->identifier->value);
        CGH->addInstruction("push", to_string(CGH->getSymbolLocation(node->identifier->value)));
        CGH->addInstruction("st");
    }
    void VisitNode(ExpressionNode* node) override {
        node->leftSimpleExpression->accept(this);
        int sizeOfRight = node->rightSimpleExpression.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightSimpleExpression[counter])->accept(this);
            get<1>(node->rightSimpleExpression[counter])->accept(this);
            counter++;
           /* symbolTable.lastExpressionType = "bool";*/
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
/*            symbolTable.lastExpressionType = "float";*/
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
    void VisitNode(FunctionCallNode* node) override{
        //CGH->addInstruction(OFRAME); // Push the current stack pointer
        if(node->actualParams){
            node->actualParams->accept(this);
        }
        CGH->addInstruction("push", "." + node->identifier->value); // instruction pointer
        CGH->addInstruction("call");
    }
    void VisitNode(ActualParamsNode* node) override {
        node->leftExpression->accept(this);
        for (const unique_ptr<ExpressionNode>& expressions : node->rightExpressions) {
            expressions->accept(this);
        }
        CGH->addInstruction("push", std::to_string(node->rightExpressions.size()+1)); // number of params
    }
    void VisitNode(PadRandINode* node) override {
        node->expression->accept(this);
        CGH->addInstruction("irnd");
    }
    void VisitNode(PadReadNode* node) override {
        get<0>(node->expressions)->accept(this);
        get<1>(node->expressions)->accept(this);
        //???
    }
    void VisitNode(LiteralNode* node) override {
        node->accept(this);
    }
    void VisitNode(IdentifierNode* node) override {}
    void VisitNode(TypeNode* node) override {}
    void VisitNode(RelationalOpNode* node) override {
        string type = node->value;
        if (type == "=="){
            CGH->addInstruction("eq");
            return;
        }
        if (type == "!="){
            CGH->addInstruction("eq");
            return;
        }
        if (type == "<"){
            CGH->addInstruction("gt");
            return;
        }
        if (type == ">"){
            CGH->addInstruction("lt");;
            return;
        }
        if (type == "<="){
            CGH->addInstruction("le");
            return;
        }
        if (type == ">="){
            CGH->addInstruction("gt");
            return;
        }
    }
    void VisitNode(AdditiveOpNode* node) override {
        string type = node->value;
        if (type == "+"){
            CGH->addInstruction("add");
            return;
        }
        if (type == "-"){
            CGH->addInstruction("sub");
            return;
        }
        if (type == "or"){
            CGH->addInstruction("max");
            return;
        }
    }
    void VisitNode(MultiplicativeOpNode* node) override {
        string type = node->value;
        if (type == "/"){
            CGH->addInstruction("div");
            return;
        }
        if (type == "*"){
            CGH->addInstruction("mul");
            return;
        }
        if (type == "and"){
            CGH->addInstruction("min");
            return;
        }
    }
    void VisitNode(BooleanLiteralNode* node) override {
        string val = node->value;
        if(val == "true"){
            CGH->addInstruction("push","1");
            return;
        }
        if(val == "false"){
            CGH->addInstruction("push","0");
            return;
        }
    }
    void VisitNode(IntegerLiteralNode* node) override {
        CGH->addInstruction("push", node->value); // Push the integer value onto the stack.
    }
    void VisitNode(FloatLiteralNode* node) override {
        CGH->addInstruction("push", node->value); // Push the floating-point value onto the stack.
    }
    void VisitNode(ColourLiteralNode* node) override {
        CGH->addInstruction("push", node->value); // Push the color value onto the stack. You may need to convert the color value to a suitable format first.
    }
    void VisitNode(PadWidthNode* node) override {
        CGH->addInstruction("width");
    }
    void VisitNode(PadHeightNode* node) override {
        CGH->addInstruction("height");
    }
private:
    unique_ptr<codeGenerationHelper> CGH = make_unique<codeGenerationHelper>();
};

int main() {
    string testString = "let x : int = ( ( 3 * 4 ) + 5 * 7 + ( 0 ) / 100 ) ;";
    //string testString = "let c : colour = g ( ) ; fun g ( ) -> int { return 1 ;  }";
    cout << "Testing: " << testString << endl;
    vector<string> splitString = split(testString, ' ');
    map<string, map<string, string>> lexerTable = loadMap(); //Loads the map from the CSV file
    map<string, string> finalStates = loadFinalStates();
    vector<string> allLetters = getListOfTokens(testString);
    vector<string> allWords = split(testString, ' ');
    vector<string> allTokens = traverseDFSA(lexerTable, finalStates, allLetters);
    vector<string> revisedTokens = reviseTokens(allTokens, allWords);


/*    for (const std::string& str : allTokens) {
        std::cout << str << " ";
    }
    cout << endl;

    for (const std::string& str : revisedTokens) {
        std::cout << str << " ";
    }
    cout << endl;*/

    system("Color 09");
    revisedTokens.emplace_back(END_OF_FILE);
    unique_ptr<Parser> parser = make_unique<Parser>(revisedTokens, splitString);
    unique_ptr<ProgramNode> tokens = parser->parseProgram();
    unique_ptr<CodeGeneration> codeGeneration = make_unique<CodeGeneration>();
    auto node = new CodeGeneration();
    tokens->accept(node);
    cout << endl;
    return 0;
}