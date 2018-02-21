#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"


class Expression
    : public Node
{
private:
    NodePtr csExpression;
    NodePtr assignment;
protected:
    Expression(NodePtr _arg1)
        : csExpression(null)
        , assignment(_arg1)
    {}
    Expression(NodePtr _arg1, NodePtr _arg2)
        : csExpression(_arg1)
        , assignment(_arg2)
    {}
public:
 
    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(csExpression != null) {
            csExpression->print();
            dst<<" , ";
        }
        assignment->print();
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }
};



// expr constant
// expr conditional
// EXPR_ASSIGNMENT
// EXPR_LOGIC_OR
// EXPR_LOGIC_AND
/// EXPR_INCLUSIVE_OR
// EXPR_AND
// EXPR_XOR
// EXPR_EQUALITY
// EXPR_RELATIONAL
// EXPR_SHIFT
// EXPR_ADD
// EXPR_MUL
// EXPR_CAST
// EXPR_UNARY
#endif
