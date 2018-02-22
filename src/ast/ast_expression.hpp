#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"


class expr : public Node {
    // EXPR : EXPR_ASSIGNMENT
    //      | EXPR ',' EXPR_ASSIGNMENT
    private:
        NodePtr rec;
        NodePtr assignment;
    protected:
        expr(NodePtr _arg1)
            : rec(NULL)
            , assignment(_arg1)
        {}
        expr(NodePtr _arg1, NodePtr _arg2)
            : rec(_arg1)
            , assignment(_arg2)
        {}
    public:
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL) {
                rec->PrettyPrint(dst);
                dst<<" , ";
            }
            assignment->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_conditional : public Node {
    //EXPR_CONDITIONAL : EXPR_LOGIC_OR
    //                 | EXPR_LOGIC_OR '?' EXPR ':' EXPR_CONDITIONAL
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    protected:
        expr_conditional(NodePtr _arg1)
            : eConditional(NULL)
            , expr(NULL)
            , eLOR(_arg1)
        {}
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
            : eConditional(_arg3)
            , expr(_arg2)
            , eLOR(_arg1)
        {}
    public:
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            eLOR->PrettyPrint(dst);
            if(eConditional != NULL) {
                // '?' EXPR ':' EXPR_CONDITIONAL
                dst<<'?';
                expr->PrettyPrint(dst);
                dst<<':';
                eConditional->PrettyPrint(dst);
            }
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_assignment : public Node {
    //EXPR_ASSIGNMENT : EXPR_CONDITIONAL
    //                | EXPR_UNARY OPR_ASSIGNMENT EXPR_ASSIGNMENT
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
                unary->PrettyPrint(dst);
                opr->PrettyPrint(dst);
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_logic_or : public Node {
    // EXPR_LOGIC_OR : EXPR_LOGIC_AND
    //               | EXPR_LOGIC_OR OP_BOR
    private:
        NodePtr exp;
        char bOR;

    protected:
        expr_logic_or(NodePtr _exp, char _bOR;)
            : exp(_exp)
            , bOR('|')
        {}

        expr_logic_or(NodePtr _exp)
            : exp(_exp)
            , bOR('')
        {}
        
    public:
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            exp->PrettyPrint(dst);
            if(bOR == '|'){
                dst<<'|';
            }
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_logic_and : public Node {
    //EXPR_LOGIC_AND : EXPR_INCLUSIVE_OR  
    //               | EXPR_LOGIC_AND OP_BAND EXPR_INCLUSIVE_OR
    private:
        NodePtr rec;
        char bAND;
        NodePtr exp;
        
    protected:
        expr_logic_and(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
            , bAND('&')
        {}

        expr_logic_and(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
            , bAND('')
        {}
        
    public:
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            exp->PrettyPrint(dst);
            if(bAND == '&'){
                rec->PrettyPrint(dst);
                dst<<'&';
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

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