#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"


class Expr
    : public Node
{
private:
    NodePtr csExpression;
    NodePtr assignment;
protected:
    Expr(NodePtr _arg1)
        : csExpression(null)
        , assignment(_arg1)
    {}
    Expr(NodePtr _arg1, NodePtr _arg2)
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


// expr conditional
class Expr_Cond
    : public Node
{
private:
    NodePtr eConditional;
    NodePtr expr;
    NodePtr eLOR;
protected:
    Expr_Cond(NodePtr _arg1)
        : eConditional(null)
        , expr(null)
        , eLOR(_arg1)
    {}
    Expr_Cond(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
        : eConditional(_arg1)
        , expr(_arg2)
        , eLOR(_arg3)
    {}
public:
 
    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(eConditional != null) {
            // EXPR_LOGIC_OR '?' EXPR ':' EXPR_CONDITIONAL
            eLOR->print();
            dst<<"?";
            expr->print();
            dst<<":";
            eConditional->print();
        }
        else{ eLOR->print(); }
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }
};

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
