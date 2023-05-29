#include "XMLGenerator.h"

void XMLVisitor::VisitNode(ASTNode* node){
   node->programNode->accept(this);
}
void XMLVisitor::VisitNode(ProgramNode* node){
    for (const unique_ptr<StatementNode>& statement : node->statements) {
        statement->accept(this);
    }
}
void XMLVisitor::VisitNode(StatementNode* node){
    node->accept(this);
}
void XMLVisitor::VisitNode(BlockNode* node){
    for (const unique_ptr<StatementNode>& statement : node->statements) {
        statement->accept(this);
    }
}
void XMLVisitor::VisitNode(FunctionDeclNode* node){
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
void XMLVisitor::VisitNode(FormalParamsNode* node){
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
void XMLVisitor::VisitNode(FormalParamNode* node){
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
void XMLVisitor::VisitNode(WhileStatementNode* node){
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
void XMLVisitor::VisitNode(ForStatementNode* node){
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
void XMLVisitor::VisitNode(IfStatementNode* node){
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
void XMLVisitor::VisitNode(RtrnStatementNode* node){
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
void XMLVisitor::VisitNode(PixelStatementNode* node){
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
void XMLVisitor::VisitNode(DelayStatementNode* node){
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
void XMLVisitor::VisitNode(PrintStatementNode* node){
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
void XMLVisitor::VisitNode(AssignmentNode* node){
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
void XMLVisitor::VisitNode(VariableDeclNode* node){
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
void XMLVisitor::VisitNode(ExpressionNode* node){
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
void XMLVisitor::VisitNode(SimpleExpressionNode* node){
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
void XMLVisitor::VisitNode(TermNode* node){
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
void XMLVisitor::VisitNode(FactorNode* node){
    node->accept(this);
}
void XMLVisitor::VisitNode(UnaryNode* node){
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
void XMLVisitor::VisitNode(SubExpressionNode* node){
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
void XMLVisitor::VisitNode(FunctionCallNode* node){
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
void XMLVisitor::VisitNode(ActualParamsNode* node){
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
void XMLVisitor::VisitNode(PadRandINode* node){
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
void XMLVisitor::VisitNode(PadReadNode* node){
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
void XMLVisitor::VisitNode(LiteralNode* node){
    node->accept(this);
}
void XMLVisitor::VisitNode(IdentifierNode* node){
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
void XMLVisitor::VisitNode(TypeNode* node){
    string tokenNode = TYPE;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value+"'");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(RelationalOpNode* node){
    string tokenNode = RELATIONAL_OP;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1," Op = '"+node->value+"'");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(AdditiveOpNode* node){
    string tokenNode = ADDITIVE_OP;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1," Op = '"+node->value+"'");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(MultiplicativeOpNode* node){
    string tokenNode = MULTIPLICATIVE_OP;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1," Op = '"+node->value+"'");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(BooleanLiteralNode* node){
    string tokenNode = BOOLEAN_LITERAL;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value)+"'";
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(IntegerLiteralNode* node){
    string tokenNode = INTEGER_LITERAL;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value+"'");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(FloatLiteralNode* node){
    string tokenNode = FLOAT_LITERAL;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value+"'");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(ColourLiteralNode* node){
    string tokenNode = COLOR_LITERAL;
    tokenNode.insert(1,"\\");
    tokenNode.insert(tokenNode.length()-1,"Val = '"+node->value);
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(PadWidthNode* node){
    string tokenNode = PAD_WIDTH;
    tokenNode.insert(1,"\\");
    indent();
    cout << tokenNode <<endl;
}
void XMLVisitor::VisitNode(PadHeightNode* node){
    string tokenNode = PAD_HEIGHT;
    tokenNode.insert(1,"\\");
    indent();
    cout << tokenNode <<endl;
}
