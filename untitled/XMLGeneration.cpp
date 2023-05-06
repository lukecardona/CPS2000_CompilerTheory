#include "parser.h"

class Visitor {
public:
    virtual void VisitProgram(ProgramNode* node) = 0;
    virtual void VisitDeclaration(VariableDeclNode* node) = 0;
    virtual void VisitStatement(StatementNode* node) = 0;
    // More visit methods for other node types
};


class XMLVisitor: public Visitor{
public:
    void VisitProgram(ProgramNode* node) override {
        indent_level += 1;
        indent();
    }
    void VisitStatement(StatementNode* node) override{

    }


    
private:
    int indent_level;
    void indent() const {
        for (int i = 0; i < indent_level * 4; ++i) {
            std::cout << ' ';
        }
    }
};





void outputXML(unique_ptr<ASTNode> const & statement){
    auto& children = statement->getChildren();
    if (!children.empty()){
        for (const auto& child : children) {
            cout << child->getNodeType();
            cout << endl;
            outputXML(child);
        }
    }
}

int main(){
    string testString = "let x : float = 3.142 * 20.0765 + 5 ;";
    cout << "Testing: " << testString << endl;
    map<string,map<string,string>> lexerTable = loadMap(); //Loads the map from the CSV file
    map<string, string> finalStates = loadFinalStates();
    vector<string> allLetters = getListOfTokens(testString);
    vector<string> allWords = split(testString,' ');
    vector<string> allTokens = traverseDFSA(lexerTable,finalStates,allLetters);
    vector<string> revisedTokens = reviseTokens(allTokens,allWords);

    for (const std::string& str : allTokens) {
        std::cout << str << " ";
    }
    cout << endl;

    for (const std::string& str : revisedTokens) {
        std::cout << str << " ";
    }
    cout << endl;

    revisedTokens.emplace_back(END_OF_FILE);
    unique_ptr<Parser> parser = make_unique<Parser>(revisedTokens);
    unique_ptr<ASTNode> tokens = parser->parseProgram();
    auto& children = tokens->getChildren();
    for (const auto& child : children) {

        // Perform operations on each child node here
        // For example, you can print the node type or any other information:
        cout << endl;
        outputXML(child);
    }
    cout << endl;
    return 0;
}