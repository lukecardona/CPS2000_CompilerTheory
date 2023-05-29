#ifndef XMLVISITOR_H
#define XMLVISITOR_H

#include "parser.h"

class XMLVisitor : public Visitor {
public:
    void VisitNode(ASTNode* node) override;
    void VisitNode(ProgramNode* node) override;
    void VisitNode(StatementNode* node) override;
    void VisitNode(BlockNode* node) override;
    void VisitNode(VariableDeclNode *node) override;
    void VisitNode(FunctionDeclNode* node) override;
    void VisitNode(FormalParamsNode* node) override;
    void VisitNode(FormalParamNode* node) override;
    void VisitNode(WhileStatementNode* node) override;
    void VisitNode(ForStatementNode* node) override;
    void VisitNode(IfStatementNode* node) override;
    void VisitNode(RtrnStatementNode* node) override;
    void VisitNode(PixelStatementNode* node) override;
    void VisitNode(DelayStatementNode* node) override;
    void VisitNode(PrintStatementNode* node) override;
    void VisitNode(AssignmentNode* node) override;
    void VisitNode(ExpressionNode* node) override;
    void VisitNode(SimpleExpressionNode* node) override;
    void VisitNode(TermNode* node) override;
    void VisitNode(FactorNode* node) override;
    void VisitNode(UnaryNode* node) override;
    void VisitNode(SubExpressionNode* node) override;
    void VisitNode(FunctionCallNode* node) override;
    void VisitNode(ActualParamsNode* node) override;
    void VisitNode(PadRandINode* node) override;
    void VisitNode(PadReadNode* node) override;
    void VisitNode(LiteralNode* node) override;
    void VisitNode(IdentifierNode* node) override;
    void VisitNode(TypeNode* node) override;
    void VisitNode(RelationalOpNode* node) override;
    void VisitNode(AdditiveOpNode* node) override;
    void VisitNode(MultiplicativeOpNode* node) override;
    void VisitNode(BooleanLiteralNode* node) override;
    void VisitNode(IntegerLiteralNode* node) override;
    void VisitNode(FloatLiteralNode* node) override;
    void VisitNode(ColourLiteralNode* node) override;
    void VisitNode(PadWidthNode* node) override;
    void VisitNode(PadHeightNode* node) override;
private:
    int indent_level{};

    void indent() const {
        for (int i = 0; i < indent_level * 4; ++i) {
            std::cout << ' ';
        }
    }


};

#endif // XMLVISITOR_H
