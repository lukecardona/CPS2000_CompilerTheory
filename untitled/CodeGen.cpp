#include <unordered_map>
#include "Sematic Anylsis.h"

stringstream ss;

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
#define STORE "st"
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

class CodeHelper {
public:
    void enterScope(){
        symbolTable.emplace_back();
        if(functionInstructionsFlag){
            numberOfOpenFunctionScopes++;
        }
    }
    void exitScope() {
        symbolTable.pop_back();
        if(functionInstructionsFlag){
            numberOfOpenFunctionScopes--;
        }
    }
    void enterFunctionScope() {
        if(functionInstructionsFlag){
            throw std::runtime_error("\033[1;31m Unable to declare a function inside a function!\033[0m");
        } //Ensures that function cannot be declared in a function
        functionInstructionsFlag = true;
    }
    void exitFunctionScope() {
        functionInstructionsFlag = false;
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
    void addSymbol(const string& name) {
        int size = symbolTable.back().size();
        symbolTable.back()[name] = size;
    }
    string findSymbol(const std::string& varName) {
        if (functionInstructionsFlag) {
            // Find symbol in currentFunctionVariableTable
            for (int i = symbolTable.size()-1; i >= 0; i--) {
                auto it = symbolTable[i].find(varName);
                if (it != symbolTable[i].end()){
                    return to_string(it->second);
                }
            }
        }
        // Find symbol in mainVariableTable
        for (int i = symbolTable.size()-1; i >= 0; i--) {
            auto it = symbolTable[i].find(varName);
            if (it != symbolTable[i].end()){
                return to_string(it->second);
            }
        }
        // Symbol not found in either table
        throw runtime_error("\033[1;31m Key: " + varName + " not found\033[0m");
    }
    string findFrame(const string& varName){
        if (functionInstructionsFlag) {
            // Find symbol in currentFunctionVariableTable
            for (int i = symbolTable.size()-1; i >= 0; i--) {
                auto it = symbolTable[i].find(varName);
                if (it != symbolTable[i].end()){
                    int size = abs(int((symbolTable.size()-1)));
                    return to_string(size);
                }
            }
        }
        // Find symbol in mainVariableTable
        for (int i = symbolTable.size()-1; i >= 0; i--) {
            auto it = symbolTable[i].find(varName);
            if (it != symbolTable[i].end()){
                int size = abs(int((symbolTable.size()-1)-i));
                return to_string(size);
            }
        }
        // Symbol not found in either table
        throw runtime_error("\033[1;31m Key: " + varName + " not found\033[0m");
    }
    int getCurrentScopeSize(){
        return symbolTable.back().size();
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
        }
    }
    int getCurrentOpenFunctionScopes() {
        return numberOfOpenFunctionScopes;
    };
private:
    vector<unordered_map<string, int>> symbolTable;
    vector<string> mainInstructions;
    vector<string> functionInstructions;
    bool functionInstructionsFlag = false;
    int numberOfOpenFunctionScopes = 0;
};
class CodeGeneratorVisitor : public Visitor{
public:
    void VisitNode(ASTNode* node) override {
        node->programNode->accept(this);
    }
    void VisitNode(FunctionDeclNode* node) override {

        if(!mainOver){
            //Append Function
            allFunctionNodes.push_back(node);
            return;
        }

        CH.enterFunctionScope(); //Start a function
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
        CH.exitFunctionScope(); //Exit Function
        currentFunctionVariables = 0;
    }
    void VisitNode(ProgramNode* node) override {
        CH.addInstruction(MAIN);
        CH.enterScope();
        CH.addInstruction(PUSH,PLACE_HOLDER);
        int initVarIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(OFRAME);

        for (const unique_ptr<StatementNode>& statement : node->statements) {
            statement->accept(this);
        }

        int currentScopeVar = CH.getCurrentScopeSize();
        if (currentScopeVar > 0){
            CH.replaceInstruction(initVarIndex,PUSH, to_string(currentScopeVar));
        }else{
            CH.deleteInstruction(initVarIndex); // Deletes push
            CH.deleteInstruction(initVarIndex); // Deletes OFrame
        }
        mainOver = true;
        //Handle Functions
        for (vector<FunctionDeclNode*>::const_iterator it = allFunctionNodes.begin(); it != allFunctionNodes.end(); ++it) {
            VisitNode(*it);
        }
        CH.addInstruction(HALT);
        CH.exitScope();
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
    void VisitNode(FormalParamsNode* node) override {
        node->formalParamLeft->accept(this);
        for (const unique_ptr<FormalParamNode>& statement : node->formalParamRight) {
            statement->accept(this);
        }
    }
    void VisitNode(FormalParamNode* node) override {}
    void VisitNode(WhileStatementNode* node) override {
        CH.addInstruction("\n//WHILE");

        CH.addInstruction(PUSH, PLACE_HOLDER);
        int pushScopeIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(OFRAME);
        CH.enterScope();
        OFrameScope++;

        CH.addInstruction(PUSH,PLACE_HOLDER);
        int firstPC = CH.getCurrentInstructionIndex();

        node->expression->accept(this);
        CH.addInstruction(JUMP_IF_1);
        CH.addInstruction(PUSH,PLACE_HOLDER);
        int startWhile = CH.getCurrentInstructionIndex();
        int jumpTo = startWhile-firstPC+2;
        CH.replaceInstruction(firstPC, PUSH, PC_COUNTER+to_string(jumpTo));
        CH.addInstruction(JUMP);

        CH.addInstruction("\n//Loop");
        node->block->accept(this);

        int currentIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(PUSH,PC_COUNTER_NEGATIVE+ to_string(currentIndex-firstPC));
        CH.addInstruction(JUMP);
        int endOfWhile = CH.getCurrentInstructionIndex();
        CH.replaceInstruction(startWhile,PUSH,PC_COUNTER+to_string(endOfWhile-startWhile));

        int scopeSize = CH.getCurrentScopeSize();
        if(scopeSize > 0){
            CH.replaceInstruction(pushScopeIndex,PUSH, to_string(scopeSize));
        }
        else{
            CH.deleteInstruction(pushScopeIndex);
        }
        CH.exitScope();
        CH.addInstruction(CFRAME);;
        OFrameScope--;
    }
    void VisitNode(ForStatementNode* node) override {
        CH.addInstruction("\n//FOR");

        CH.addInstruction(PUSH, PLACE_HOLDER);
        int pushScopeIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(OFRAME);
        CH.enterScope();
        OFrameScope++;

        if (node->varDecl){
            CH.addInstruction("//FOR DECL");
            node->varDecl->accept(this);
        } //If there exists a variable declaration


        CH.addInstruction("\n//FOR CHECK");

        CH.addInstruction(PUSH,PLACE_HOLDER);
        int loopStartId = CH.getCurrentInstructionIndex();

        node->expression->accept(this);
        CH.addInstruction(JUMP_IF_1);

        int expressionId = CH.getCurrentInstructionIndex();

        CH.addInstruction(PUSH,PLACE_HOLDER);
        int PC2 = CH.getCurrentInstructionIndex();
        CH.addInstruction(JUMP);

        int curIn = CH.getCurrentInstructionIndex();
        CH.replaceInstruction(loopStartId,PUSH,PC_COUNTER+to_string(curIn-loopStartId+1));

        CH.addInstruction("\n//FOR Loop");
        node->block->accept(this);

        if (node->assignment) {
            CH.addInstruction("\n//FOR Assignment");
            node->assignment->accept(this);
        } //If there exists a nose assignment

        int currentIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(PUSH,PC_COUNTER_NEGATIVE+ to_string(currentIndex-loopStartId));
        CH.addInstruction(JUMP);
        currentIndex = CH.getCurrentInstructionIndex();
        CH.replaceInstruction(PC2,PUSH,PC_COUNTER+ to_string(currentIndex-PC2));
        int scopeSize = CH.getCurrentScopeSize();
        if(scopeSize > 0){
            CH.replaceInstruction(pushScopeIndex,PUSH, to_string(scopeSize));
        }
        else{
            CH.deleteInstruction(pushScopeIndex-1);
        }

        CH.exitScope();
        CH.addInstruction(CFRAME);;
        OFrameScope--;
    }
    void VisitNode(IfStatementNode* node) override {
        CH.addInstruction("\n//IF");
        CH.addInstruction(PUSH, PLACE_HOLDER);
        int pushScopeIndex = CH.getCurrentInstructionIndex();
        CH.addInstruction(OFRAME);
        CH.enterScope();
        OFrameScope++;

        int firstPC = CH.getCurrentInstructionIndex();
        CH.addInstruction(PUSH,PLACE_HOLDER);


        node->expression->accept(this);
        CH.addInstruction(JUMP_IF_1);

        int secondPC = CH.getCurrentInstructionIndex();
        CH.addInstruction(PUSH,PLACE_HOLDER);
        CH.addInstruction(JUMP);

        int startIf = CH.getCurrentInstructionIndex();
        int jumpTo = startIf-firstPC;
        CH.replaceInstruction(firstPC+1, PUSH, PC_COUNTER+to_string(jumpTo));

        node->ifBlock->accept(this);

        int endIf = CH.getCurrentInstructionIndex();
        CH.replaceInstruction(secondPC+1,PUSH,PC_COUNTER+to_string(endIf-secondPC)+"//Jump to Else");

        int ifScopeSize = CH.getCurrentScopeSize();

        if(node->elseBlock){
            CH.exitScope();
            CH.addInstruction(PUSH,PLACE_HOLDER);
            int PC3 = CH.getCurrentInstructionIndex();
            CH.addInstruction(JUMP);
            CH.replaceInstruction(secondPC+1,PUSH,PC_COUNTER+to_string(endIf-secondPC+2)+"//Jump to Else");
            CH.enterScope();

            node->elseBlock->accept(this);
            int endElse = CH.getCurrentInstructionIndex();
            CH.replaceInstruction(PC3,PUSH, PC_COUNTER+to_string(endElse-PC3+1)+"//Jump to end of if");
        }

        int elseScopeSize = CH.getCurrentScopeSize();
        int scopeSize;
        if(ifScopeSize >= elseScopeSize){
            scopeSize = ifScopeSize;
        }else{
            scopeSize = elseScopeSize;
        }

        if(scopeSize > 0){
            CH.replaceInstruction(pushScopeIndex,PUSH, to_string(scopeSize));
        }
        else{
            CH.deleteInstruction(pushScopeIndex);
        }

        CH.exitScope();
        CH.addInstruction(CFRAME);;
        OFrameScope--;
    }
    void VisitNode(RtrnStatementNode* node) override {
        node->expression->accept(this);
        int scopes = CH.getCurrentOpenFunctionScopes();
        while (scopes > 0){
            CH.addInstruction(CFRAME);
            scopes--;
        }
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
            get<2>(node->pixel)->accept(this);
            get<1>(node->pixel)->accept(this);
            get<0>(node->pixel)->accept(this);
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
        string memoryLocation = CH.findSymbol(node->identifier->value);
        CH.addInstruction(PUSH, memoryLocation); // target location
        CH.addInstruction(PUSH, CH.findFrame(node->identifier->value)); // target location
        CH.addInstruction(STORE);
    }
    void VisitNode(VariableDeclNode* node) override {
        CH.addSymbol(node->identifier->value);
        node->expression->accept(this);
        CH.addInstruction(PUSH, CH.findSymbol(node->identifier->value));
        CH.addInstruction(PUSH, CH.findFrame(node->identifier->value));
        CH.addInstruction(STORE);
    }
    void VisitNode(ExpressionNode* node) override {
        int sizeOfRight = node->rightSimpleExpression.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<1>(node->rightSimpleExpression[counter])->accept(this); //ReversedOrder
            counter++;
        }

        node->leftSimpleExpression->accept(this);

        sizeOfRight = node->rightSimpleExpression.size();
        counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightSimpleExpression[counter])->accept(this);
            counter++;
        }
    }
    void VisitNode(SimpleExpressionNode* node) override {
        //Change Type of transition for every node
        int sizeOfRight = node->rightTerm.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<1>(node->rightTerm[counter])->accept(this);
            counter++;
        }

        node->leftTerm->accept(this);
        sizeOfRight = node->rightTerm.size();
        counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightTerm[counter])->accept(this); //ReversedOrder
            counter++;
        }

    }
    void VisitNode(TermNode* node) override {
        int sizeOfRight = node->rightFactor.size();
        int counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<1>(node->rightFactor[counter])->accept(this);
            counter++;
        }

        //Change Type of transition for every node
        node->leftFactor->accept(this);
        sizeOfRight = node->rightFactor.size();
        counter = 0;
        while (sizeOfRight > 0){
            sizeOfRight--;
            get<0>(node->rightFactor[counter])->accept(this); //ReversedOrder
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
    void VisitNode(PadReadNode* node) override {
        get<0>(node->expressions)->accept(this);
        get<1>(node->expressions)->accept(this);
    }
    void VisitNode(LiteralNode* node) override {
        node->accept(this);
    }
    void VisitNode(IdentifierNode* node) override {
        CH.addInstruction(PUSH,"["+CH.findSymbol(node->value)+":"+ CH.findFrame(node->value)+"]");
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
            CH.addInstruction("lt");
            return;
        }
        if (type == ">"){
            CH.addInstruction("gt");
            CH.addInstruction(PUSH,"0");
            CH.addInstruction("eq");;
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
    vector<FunctionDeclNode*> allFunctionNodes;
    bool inAFunction = false;
    bool mainOver = false;
};

int main(){
    //Lexer Stuff
    Lexer lex = Lexer();

    string path;
    cout << "Please enter relative path to test: "; // Type a number and press enter
    cin >> path; // Get user input from the keyboard/

    path = "../"+path;
    cout << path;

    lex.LexerPassFile(path);

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

    unique_ptr<XMLVisitor> xmlVisitor = make_unique<XMLVisitor>();
    xmlVisitor->VisitNode(parsedProgram);

    cout << endl << "\n\n===Generated code===\n\n" << endl;

    unique_ptr<CodeGeneratorVisitor> codeGeneratorVisitor = make_unique<CodeGeneratorVisitor>();
    codeGeneratorVisitor->loadParsedTokens(parsedProgram);
    return 0;
}