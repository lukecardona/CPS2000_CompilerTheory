#include <unordered_map>
#include "parser.h"

#define OFRAME "oframe"
#define CFRAME "cframe"
#define ALLOC "alloc"

#define PUSH "push"
#define NOP "nop"
#define DROP "drop"

#define ADD "add"
#define SUBTRACTION "sub"
#define DIVISION "div"
#define INC "inc"
#define DEC "dec"
#define MAX "max"
#define MIN "min"
#define RANDOM_NUM "irnd"
#define JUMP "jmp"
#define JUMP_IF_1 "cjmp"
#define END_OF_JUMP "cjmp2"
#define CALL "call"
#define RETURN "ret"
#define STORE "str"
#define HALT "halt"

#define EQUAL "eq"
#define LESS_THAN "lt"
#define PLACE_HOLDER "<PlaceHolder>"
#define DELAY "delay"
#define PRINT "print"
#define PIXEL "pixel"
#define PIXELR "pixelr"
#define CLEAR "clear"
#define WIDTH "width"
#define HEIGHT "height"
#define MAIN ".main"
#define PC_COUNTER "#PC+"
#define PC_COUNTER_NEGATIVE "#PC-"
class SymbolTable {
public:
    void enterScope() {
        symbolTable.emplace_back();
    }

    void exitScope() {
        symbolTable.pop_back();
    }

    bool isDefinedInCurrentScope(const string &name) const {
        for (const auto &scope : symbolTable) {
            if (scope.find(name) != scope.end()) {
                return true;
            }
        }
        return false;
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
class CodeHelper {
public:
    void enterScope() {
        if(functionInstructionsFlag){
            throw std::runtime_error("\033[1;31m Unable to declare a function inside a function!\033[0m");
        } //Ensures that function cannot be declared in a function
        functionInstructionsFlag = true;
    }
    void exitScope() {
        functionInstructionsFlag = false;
        currentFunctionVariableTable.clear();
    }
    void addInstruction(const string& inst) {
        if (functionInstructionsFlag) {
            functionInstructions.push_back(inst);
        } else {
            mainInstructions.push_back(inst);
        }
    }
    void addInstruction(const string& inst1, const string& inst2){
        if (functionInstructionsFlag) {
            functionInstructions.push_back(inst1+" "+inst2);
        } else {
            mainInstructions.push_back(inst1+" "+inst2);
        }
    }
    void replaceInstruction(int index,const string& inst,const string& inst2){
        if(functionInstructionsFlag){
            functionInstructions[index-1] = inst+" "+inst2;
        }else{
            mainInstructions[index-1] = inst+" "+inst2;
        }
    }
    int getCurrentInstructionIndex(){
        if (functionInstructionsFlag){
            return functionInstructions.size();
        }else{
            return mainInstructions.size();
        }
    }
    bool getFunctionFlag() const{
        return functionInstructionsFlag;
    }
    void addSymbol( const string& varName, int memoryIndex){
        if(functionInstructionsFlag){
            currentFunctionVariableTable[varName] = memoryIndex;
        }else{
            mainVariableTable[varName] = memoryIndex;
        }
    }
    int findSymbol(const std::string& varName) {
        if (functionInstructionsFlag) {
            // Find symbol in currentFunctionVariableTable
            auto it = currentFunctionVariableTable.find(varName);
            if (it != currentFunctionVariableTable.end())
                return it->second;  // Return the associated value
//            else
//throw runtime_error("\033[1;31m Key: " + varName + " not found in currentFunctionVariableTable \033[0m");
        } else {
            // Find symbol in mainVariableTable
            auto it = mainVariableTable.find(varName);
            if (it != mainVariableTable.end())
                return it->second;  // Return the associated value
  //          else
  //              throw runtime_error("\033[1;31m Key" + varName + " not found in mainVariableTable \033[0m");
        }
    }
    void generateInstructions(){
        cout << endl;
        for (const std::string& str : mainInstructions) {
            cout << str << endl;
        }
        cout << endl;
        for (const std::string& str : functionInstructions) {
            cout << str << endl;
        }
        cout << endl;
    }
    void deleteInstruction(int index){
        if(functionInstructionsFlag){
            functionInstructions.erase(functionInstructions.begin()+(index-1));
        }else{
            mainInstructions.erase(mainInstructions.begin()+(index-1));
        };
    }
private:
    unordered_map<string, int> mainVariableTable;
    unordered_map<string, int> currentFunctionVariableTable;
    vector<string> mainInstructions;
    vector<string> functionInstructions;
    bool functionInstructionsFlag = false;
};

class CodeGeneratorVisitor : public Visitor{
public:
    void VisitNode(ASTNode* node) override {
        node->programNode->accept(this);
    }
    void VisitNode(ProgramNode* node) override {
        //Semantic Analysis
        CH.addInstruction(MAIN);
        CH.addInstruction(PUSH,PLACE_HOLDER);
        int initVarIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(OFRAME);

        for (const unique_ptr<StatementNode>& statement : node->statements) {
            statement->accept(this);
        }

        if (currentMainVariables > 0){
            CH.replaceInstruction(initVarIndex,PUSH, to_string(currentMainVariables));
        }else{
            CH.deleteInstruction(initVarIndex); // Deletes push
            CH.deleteInstruction(initVarIndex); // Deletes OFrame
        }
        CH.addInstruction(HALT);
        CH.generateInstructions();
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
        CH.enterScope(); //Start a function
        currentFunctionVariables = 0;
        CH.addInstruction("."+node->identifier->value); //Add the identifier of the function-

        CH.addInstruction(PUSH,PLACE_HOLDER); //Push Number of Variables to be declared
        int functionStartIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(ALLOC);

        if(node->formalParams){ // Parse the parameters
            node->formalParams->accept(this);
        }

        node->block->accept(this);

        if(currentFunctionVariables > 0){
            CH.replaceInstruction(functionStartIndex,PUSH, to_string(currentFunctionVariables));
        }else{
            CH.deleteInstruction(functionStartIndex); //Deletes Push
            CH.deleteInstruction(functionStartIndex); //Deletes Alloc
        }
        CH.exitScope(); //Exit Function
        currentFunctionVariables = 0;
    }
    void VisitNode(FormalParamsNode* node) override {
        node->formalParamLeft->accept(this);
        for (const unique_ptr<FormalParamNode>& statement : node->formalParamRight) {
            statement->accept(this);
        }
    }
    void VisitNode(FormalParamNode* node) override {}
    void VisitNode(WhileStatementNode* node) override {
        int loopStartId = CH.getCurrentInstructionIndex();

        node->expression->accept(this); //Less then, Expression, greater then

        CH.addInstruction(PUSH, "0"); //Compare with false
        CH.addInstruction(EQUAL); //Check if equal to false
        CH.addInstruction(JUMP_IF_1,PLACE_HOLDER);
        int jumpAfterLoopIdx = CH.getCurrentInstructionIndex(); //Not sure if +1 is needed
        CH.addInstruction(END_OF_JUMP);

        node->block->accept(this);

        int currentIndex =  CH.getCurrentInstructionIndex();  //Not sure if +1 is needed
        string jmpInst = PC_COUNTER+to_string(currentIndex-jumpAfterLoopIdx);
        CH.replaceInstruction(jumpAfterLoopIdx,PUSH, jmpInst);

        jmpInst = to_string(currentIndex-loopStartId);
        CH.addInstruction(END_OF_JUMP, PC_COUNTER_NEGATIVE+jmpInst);
    }
    void VisitNode(ForStatementNode* node) override {
        if (node->varDecl){
            node->varDecl->accept(this);
        } //If there exists a variable declaration

        int loopStartId = CH.getCurrentInstructionIndex();
        node->expression->accept(this);
        CH.addInstruction(PUSH, "0"); //Compare with false
        CH.addInstruction(EQUAL); //Check if equal to false
        CH.addInstruction(JUMP_IF_1,PLACE_HOLDER);
        int jumpAfterLoopIdx = CH.getCurrentInstructionIndex(); //Not sure if +1 is needed

        if (node->assignment) {
            node->assignment->accept(this);
        } //If there exists a nose assignment
        node->block->accept(this);
        CH.addInstruction(JUMP, to_string(loopStartId));
        CH.replaceInstruction(jumpAfterLoopIdx,JUMP_IF_1, to_string(CH.getCurrentInstructionIndex()));
    }
    void VisitNode(IfStatementNode* node) override {
        node->expression->accept(this);
        CH.addInstruction(PUSH, "0"); //Compare with false
        CH.addInstruction(EQUAL); //Check if equal to false
        CH.addInstruction(JUMP_IF_1,PLACE_HOLDER);
        int jumpToElseIdx = CH.getCurrentInstructionIndex(); //Not sure if +1 is needed
        CH.addInstruction(END_OF_JUMP);

        node->ifBlock->accept(this);

        int endOfIf =  CH.getCurrentInstructionIndex();  //Not sure if +1 is needed
        string jmpInst = PC_COUNTER+to_string(endOfIf-jumpToElseIdx);
        CH.replaceInstruction(jumpToElseIdx,PUSH, jmpInst);

        if (node->elseBlock) {
            CH.addInstruction(JUMP,PLACE_HOLDER);//Jumps to end of else
            int startOfElse = CH.getCurrentInstructionIndex();
            CH.addInstruction(JUMP);
            node->elseBlock->accept(this);
            int currentIndex = CH.getCurrentInstructionIndex();
            jmpInst = PC_COUNTER+to_string(currentIndex-startOfElse);
            CH.replaceInstruction(startOfElse,PUSH, jmpInst);
        } //If an else block is present
    }
    void VisitNode(RtrnStatementNode* node) override {
        node->expression->accept(this);
        CH.addInstruction("ret");
    }
    void VisitNode(PixelStatementNode* node) override {
        if(get<0>(node->pixel_r)){
            get<0>(node->pixel_r)->accept(this);
            get<1>(node->pixel_r)->accept(this);
            get<2>(node->pixel_r)->accept(this);
            get<3>(node->pixel_r)->accept(this);
            get<4>(node->pixel_r)->accept(this);
            CH.addInstruction(PIXELR);
            return;
        }
        if(get<0>(node->pixel)){
            get<0>(node->pixel)->accept(this);
            get<1>(node->pixel)->accept(this);
            get<2>(node->pixel)->accept(this);
            CH.addInstruction(PIXEL);
            return;
        }
        throw runtime_error("\033[1;31m Code Generation Error: Pixel Statement unable to be generated \033[0m");
    }//Not 100% confident on implementation
    void VisitNode(DelayStatementNode* node) override {
        node->expression->accept(this);
        CH.addInstruction(DELAY);
    }
    void VisitNode(PrintStatementNode* node) override {
        node->expression->accept(this);
        CH.addInstruction(PRINT);
    }
    void VisitNode(AssignmentNode* node) override {
        node->expression->accept(this); // evaluate the expression
        int memoryLocation = CH.findSymbol(node->identifier->value);
        CH.addInstruction(PUSH, to_string(memoryLocation)); // target location
        CH.addInstruction(STORE);
    }
    void VisitNode(VariableDeclNode* node) override {
        node->expression->accept(this);
        if(CH.getFunctionFlag()){
            CH.addSymbol(node->identifier->value,currentFunctionVariables+currentMainVariables); //Will Cause Errors
            CH.addInstruction(PUSH, to_string(currentFunctionVariables+currentMainVariables));
            CH.addInstruction(PUSH, to_string(OFrameScope));
            CH.addInstruction(STORE);
            currentFunctionVariables++;
        }else{
            CH.addSymbol(node->identifier->value,currentMainVariables);
            CH.addInstruction(PUSH, to_string(currentMainVariables));
            CH.addInstruction(PUSH, to_string(OFrameScope));
            CH.addInstruction(STORE);
            currentMainVariables++;
        }
    }
    void VisitNode(ExpressionNode* node) override {
        node->leftSimpleExpression->accept(this);
        int sizeOfRight = node->rightSimpleExpression.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightSimpleExpression[counter])->accept(this);
            counter++;
        }
        sizeOfRight = node->rightSimpleExpression.size();
        counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<1>(node->rightSimpleExpression[counter])->accept(this); //ReversedOrder
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
            counter++;
        }
        sizeOfRight = node->rightTerm.size();
        counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<1>(node->rightTerm[counter])->accept(this); //ReversedOrder
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
            counter++;
        }
        sizeOfRight = node->rightFactor.size();
        counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<1>(node->rightFactor[counter])->accept(this); //ReversedOrder
            counter++;
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
        if(node->actualParams){
            node->actualParams->accept(this);
        }
        CH.addInstruction(PUSH, "." + node->identifier->value); // instruction pointer
        CH.addInstruction(CALL);
    }
    void VisitNode(ActualParamsNode* node) override {
        node->leftExpression->accept(this);
        for (const unique_ptr<ExpressionNode>& expressions : node->rightExpressions) {
            expressions->accept(this);
        }
        CH.addInstruction(PUSH, to_string(node->rightExpressions.size()+1)); // number of params
    }
    void VisitNode(PadRandINode* node) override {
        node->expression->accept(this);
        CH.addInstruction("irnd");
    }
    void VisitNode(PadReadNode* node) override {}
    void VisitNode(LiteralNode* node) override {
        node->accept(this);
    }
    void VisitNode(IdentifierNode* node) override {
        CH.addInstruction(PUSH,"["+ to_string(CH.findSymbol(node->value)) +"]");
    }
    void VisitNode(TypeNode* node) override {}
    void VisitNode(RelationalOpNode* node) override {
        string type = node->value;
        if (type == "=="){
            CH.addInstruction("eq //Relation Op");
            return;
        }
        if (type == "!="){
            CH.addInstruction("eq");
            return;
        }
        if (type == "<"){
            CH.addInstruction("gt");
            return;
        }
        if (type == ">"){
            CH.addInstruction("lt");;
            return;
        }
        if (type == "<="){
            CH.addInstruction("le");
            return;
        }
        if (type == ">="){
            CH.addInstruction("gt");
            return;
        }
    }
    void VisitNode(AdditiveOpNode* node) override {
        string type = node->value;
        if (type == "+"){
            CH.addInstruction("add");
            return;
        }
        if (type == "-"){
            CH.addInstruction("sub");
            return;
        }
        if (type == "or"){
            CH.addInstruction("max");
            return;
        }
    }
    void VisitNode(MultiplicativeOpNode* node) override {
        string type = node->value;
        if (type == "/"){
            CH.addInstruction("div");
            return;
        }
        if (type == "*"){
            CH.addInstruction("mul");
            return;
        }
        if (type == "and"){
            CH.addInstruction("min");
            return;
        }
    }
    void VisitNode(BooleanLiteralNode* node) override {
        string val = node->value;
        if(val == "true"){
            CH.addInstruction(PUSH,"1");
            return;
        }
        if(val == "false"){
            CH.addInstruction(PUSH,"0");
            return;
        }
    }
    void VisitNode(IntegerLiteralNode* node) override {
        CH.addInstruction(PUSH, node->value); // Push the integer value onto the stack.
    }
    void VisitNode(FloatLiteralNode* node) override {
        CH.addInstruction(PUSH, node->value); // Push the floating-point value onto the stack.
    }
    void VisitNode(ColourLiteralNode* node) override {
        CH.addInstruction(PUSH,node->value); // Push the color value onto the stack. You may need to convert the color value to a suitable format first.
    }
    void VisitNode(PadWidthNode* node) override {
        CH.addInstruction(WIDTH);
    }
    void VisitNode(PadHeightNode* node) override {
        CH.addInstruction(HEIGHT);
    }

    void loadParsedTokens(ProgramNode* node){
        VisitNode(node);
    }

private:
    CodeHelper CH;
    int currentFunctionVariables = 0;
    int currentMainVariables = 0;
    int OFrameScope = 0;
    SymbolTable symbolTable;
    bool inAFunction = false;
};

int main(){
    //Lexer Stuff
    Lexer lex = Lexer();
    lex.LexerPassFile("../testExample.txt");
    vector<string> programTokenized = lex.getProgramVector();
    vector<string> revisedTokens = lex.getRevisedTokens();
    string programString = lex.getProgramString();

    cout << programString << endl;
    for (const std::string& str : revisedTokens) {
        std::cout << str << " ";
    }

    //Parse Program
    unique_ptr<Parser> parser = make_unique<Parser>(revisedTokens, programTokenized);
    unique_ptr<ProgramNode> tokens = parser->parseProgram();
    ProgramNode* parsedProgram = tokens.get();  // Converting to raw pointer
    unique_ptr<CodeGeneratorVisitor> codeGeneratorVisitor = make_unique<CodeGeneratorVisitor>();
    codeGeneratorVisitor->loadParsedTokens(parsedProgram);
    return 0;
}

