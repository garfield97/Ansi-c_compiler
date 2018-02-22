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
    
        std::string name = "expr";

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
    
        std::string name = "expr_conditional";

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
            : unary(NULL)
            , opr(NULL)
            , exp(_arg1)
        {}
        expr_assignment(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
            : unary(_arg1)
            , opr(_arg2)
            , exp(_arg3)
        {}
    public:

        std::string name = "expr_assignment";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(unary != NULL) {
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

class opr_assignment: public Node {
    private:
        std::string opr;
    protected:
        assign(std::string _opr)
            : opr(_opr)
        {}
        
    public:
        std::string name = "opr_assignment";

        std::string getOpr(){
            return opr;
        }
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<opr<<' ';
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_logic_or : public Node {
    // EXPR_LOGIC_OR : EXPR_LOGIC_AND
    //               | EXPR_LOGIC_OR OP_LOR EXPR_LOGIC_AND
    private:
        NodePtr rec;
        NodePtr exp;
        
    protected:
        expr_logic_or(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

        expr_logic_or(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
        {}
        
    public:
    
        std::string name = "expr_logic_or";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'&& ';
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_logic_and : public Node {
    //EXPR_LOGIC_AND : EXPR_INCLUSIVE_OR  
    //               | EXPR_LOGIC_AND OP_LAND EXPR_INCLUSIVE_OR
    private:
        NodePtr rec;
        NodePtr exp;
        
    protected:
        expr_logic_and(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

        expr_logic_and(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
        {}
        
    public:

        std::string name = "expr_logic_and";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'&& ';
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_inclusive_or : public Node {
    //EXPR_INCLUSIVE_OR : EXPR_XOR 
    //                  | EXPR_INCLUSIVE_OR OP_BOR EXPR_XOR
    private:
        NodePtr rec;
        NodePtr exp;
        
    protected:
        expr_inclusive_or(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

        expr_inclusive_or(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
        {}
        
    public:
    
        std::string name = "expr_inclusive_or";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'| ';
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_xor : public Node {
    //EXPR_XOR : EXPR_AND
    //         | EXPR_XOR OP_BXOR EXPR_AND
    private:
        NodePtr rec;
        NodePtr exp;
        
    protected:
        expr_xor(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

        expr_xor(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_xor";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'^ ';
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_and : public Node {
    //EXPR_AND : EXPR_EQUALITY
    //         | EXPR_AND OP_BAND EXPR_EQUALITY
    private:
        NodePtr rec;
        NodePtr exp;
        
    protected:
        expr_and(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

        expr_and(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_and";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'& ';
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_equality : public Node {
    //EXPR_EQUALITY : EXPR_RELATIONAL
    //              | EXPR_EQUALITY OP_EQ EXPR_RELATIONAL
    //              | EXPR_EQUALITY OP_NE EXPR_RELATIONAL
    private:
        NodePtr rec;
        std::string op;
        NodePtr exp;
        
    protected:
        expr_equality(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

        expr_equality(NodePtr _exp)
            : rec(NULL)
            , op(NULL)
            , exp(_exp)
        {}
        
    public:
    
        std::string name = "expr_equality";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<op<<" ";
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_relational : public Node {
    //EXPR_RELATIONAL : EXPR_SHIFT
    //            | EXPR_RELATIONAL OP_L EXPR_SHIFT
    //            | EXPR_RELATIONAL OP_G EXPR_SHIFT
    //            | EXPR_RELATIONAL OP_LE EXPR_SHIFT
    //            | EXPR_RELATIONAL OP_GE EXPR_SHIFT
    private:
        NodePtr rec;
        std::string op;
        NodePtr exp;
        
    protected:
        expr_relational(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

        expr_relational(NodePtr _exp)
            : rec(NULL)
            , op(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_relational";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<op<<" ";
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_shift : public Node {
    //EXPR_SHIFT : EXPR_ADD
    //       | EXPR_SHIFT OP_BLEFT EXPR_ADD
    //       | EXPR_SHIFT OP_BRIGHT EXPR_ADD
    private:
        NodePtr rec;
        std::string op;
        NodePtr exp;
        
    protected:
        expr_shift(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

        expr_shift(NodePtr _exp)
            : rec(NULL)
            , op(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_shift";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<op<<" ";
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_add : public Node {
    //EXPR_ADD : EXPR_MUL
    //         | EXPR_ADD OP_PLUS EXPR_MUL
    //         | EXPR_ADD OP_MINUS EXPR_MUL
    private:
        NodePtr rec;
        std::string op;
        NodePtr exp;
        
    protected:
        expr_add(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

       expr_add(NodePtr _exp)
            : rec(NULL)
            , op(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_add";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<op<<" ";
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_mul : public Node {
    //EXPR_MUL : EXPR_CAST
    //         | EXPR_MUL OP_MUL EXPR_CAST
    //         | EXPR_MUL OP_DIV EXPR_CAST
    //         | EXPR_MUL OP_MOD EXPR_CAST
    private:
        NodePtr rec;
        std::string op;
        NodePtr exp;
        
    protected:
        expr_mul(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

        expr_mul(NodePtr _exp)
            : rec(NULL)
            , op(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_mul";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<op<<" ";
            }
            exp->PrettyPrint(dst);
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_cast : public Node {
    //EXPR_CAST : EXPR_UNARY
    //          | L_BRACKET NAME_TYPE R_BRACKET EXPR_CAST
    private:
        NodePtr rec;
        NodePtr exp;
        
    protected:
        expr_cast(NodePtr _exp, NodePtr _rec)
            : rec(_rec)
            , exp(_exp)
        {}

        expr_cast(NodePtr _exp)
            : rec(NULL)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_cast";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(rec != NULL){
                // L_BRACKET NAME_TYPE R_BRACKET EXPR_CAST
                dst<<op<<"(";
                exp->PrettyPrint(dst);
                dst<<op<<")";
                rec->PrettyPrint(dst);
            }
            else {
                exp->PrettyPrint(dst);
            }
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_unary : public Node {
    //EXPR_UNARY : EXPR_POSTFIX 
    //           | OP_INC EXPR_UNARY
    //           | OP_DEC EXPR_UNARY
    //           | OPR_UNARY EXPR_CAST
    //           | SIZEOF EXPR_UNARY
    //           | SIZEOF L_BRACKET NAME_TYPE R_BRACKET  
    private:
        NodePtr terminal;
        NodePtr exp;
        
    protected:
        expr_cast(NodePtr _terminal, NodePtr _exp)
            : terminal(_terminal)
            , exp(_exp)
        {}

        expr_cast(NodePtr _exp)
            : terminal(NULL)
            , exp(_exp)
        {}

    public:
    
        std::string name = "expr_unary";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(terminal != NULL){
                terminal->PrettyPrint();
                if(terminal->name = "name_type") dst<<' ( ';
            }
            exp->PrettyPrint();
            if(terminal != NULL && terminal->name = "name_type") dst<<' ) ';
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class opr_unary: public Node {
    private:
        std::string opr;
    protected:
        assign(std::string _opr)
            : opr(_opr)
        {}
        
    public:

        std::string name = "opr_unary";

        std::string getOpr(){
            return opr;
        }
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<opr<<' ';
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class arg_expr_list : public Node {
    //ARG_EXPR_LIST : EXPR_ASSIGNMENT
    //              | ARG_EXPR_LIST ',' EXPR_ASSIGNMENT
    private:
        NodePtr next;
        NodePtr exp;
        
    protected:
        expr_cast(NodePtr _next, NodePtr _exp)
            : next(_next)
            , exp(_exp)
        {}

        expr_cast(NodePtr _exp)
            : next(NULL)
            , exp(_exp)
        {}

    public:
    
        std::string name = "arg_expr_list";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(next != NULL){
                next->PrettyPrint();
                dst<<' , ';
            }
            exp->PrettyPrint();
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class expr_postfix : public Node {
    //EXPR_POSTFIX : EXPR_PRIMARY
    //             | EXPR_POSTFIX L_SQUARE EXPR R_SQUARE
    //             | EXPR_POSTFIX L_BRACKET R_BRACKET
    //             | EXPR_POSTFIX L_BRACKET ARG_EXPR_LIST R_BRACKET
    //             | EXPR_POSTFIX '.' IDENTIFIER
    //             | EXPR_POSTFIX OP_PTR IDENTIFIER
    //             | EXPR_POSTFIX OP_INC
    //             | EXPR_POSTFIX OP_DEC
    private:
        NodePtr next;
        std::string opr;
        std::string id;
        bool bracket;
        NodePtr exp;
        
    protected:
        expr_postfix(NodePtr _exp)// expr_primary
            : next()
            , opr()
            , id()
            , bracket()
            , exp()
        {}

        expr_postfix(NodePtr _next, NodePtr _exp)// LS exp RS
            : next()
            , opr()
            , id()
            , bracket()
            , exp()
        {}

        expr_postfix(NodePtr _next, bool _bracket)// L R
            : next()
            , opr()
            , id()
            , bracket()
            , exp()
        {}

        expr_postfix(NodePtr _next, bool _bracket, NodePtr _exp)// L AEL R
            : next()
            , opr()
            , id()
            , bracket()
            , exp()
        {}

        expr_postfix(NodePtr _next, std::string _opr, std::string _id) // OP_PTR '.'
            : next()
            , opr()
            , id()
            , bracket()
            , exp()
        {}

        expr_postfix(NodePtr _next, std::string _opr) // OP_INC OP_DEC
            : next()
            , opr()
            , id()
            , bracket()
            , exp()
        {}   

    public:
    
        std::string name = "expr_postfix";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
           
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};
#endif