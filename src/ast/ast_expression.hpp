#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"


class expr : public Node {
    private:
        NodePtr csExpression;
        NodePtr assignment;
    protected:
        expr(NodePtr _arg1)
            : csExpression(null)
            , assignment(_arg1)
        {}
        expr(NodePtr _arg1, NodePtr _arg2)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_assignment : public Node {
    private:
        NodePtr unary;
        NodePtr opr;
        NodePtr exp;
    protected:
        expr_assignment(NodePtr _arg1)
            : unary(null)
            , opr(null)
            , exp(_arg1)
        {}
        expr_assignment(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
            : unary(_arg1)
            , opr(_arg2)
            , exp(_arg3)
        {}
    public:
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(unary != null) {
                // EXPR_UNARY OPR_ASSIGNMENT EXPR_ASSIGNMENT
                unary->print();
                opr->print();
            }
            exp->print();
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_LOGIC_OR
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_LOGIC_AND
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
/// EXPR_INCLUSIVE_OR
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_AND
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_XOR
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_EQUALITY
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_RELATIONAL
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_SHIFT
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_ADD
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_MUL
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_CAST
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
// EXPR_UNARY
class expr_conditional : public Node {
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(null)
            , expr(null)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
#endif
