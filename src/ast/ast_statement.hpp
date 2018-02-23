#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"






class statement_compound : public Node{
//LIST_STATEMENT : STATEMENT
//               | LIST_STATEMENT STATEMENT
                  

private:

    NodePtr current;
    NodePtr next;



protected:

    statement_compound()
        :current(NULL)
        ,next(NULL)
    {}


    statement_compound(NodePtr _arg)
        :current(_arg)
        ,next(NULL)
    {}


    statement_compound(NodePtr _arg1,NodePtr _arg2)
        :current(_arg1)
        ,next(_arg2)
    {}
    

public:
    
    
        std::string name = "statement_compound";
        
        virtual void PrettyPrint(std::ostream &dst) const override{
          
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


class list_statement : public Node{

//LIST_STATEMENT : STATEMENT
//               | LIST_STATEMENT STATEMENT
                  

private:

    NodePtr current;
    NodePtr next;


protected:
    
    list_statement(NodePtr _arg1)
        :current(_arg1)
        ,next(NULL)
    {}
    
    list_statement(NodePtr _arg1,NodePtr _arg2)
        :current(_arg1)
        ,next(_arg2)
    {}

public:

        std::string name = "list_statement";
    
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




class statement_expr : public Node{

//STATEMENT_EXPR : ';'
//               | EXPR ';'


private:

    NodePtr current

protected:
        //since first line its no non terminal matched, terminal symbol only so maybe empty set?

    statement_expr()
        :current(NULL)
    {}
    
    statement_expr(NodePtr _arg1)
        :current(NodePtr _arg1)

    {}
    
public:


        std::string name = "statement_expr";
    
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
    
    
    
class statement : public Node{

//STATEMENT : STATEMENT_LABELED
//          | STATEMENT_COMPOUND
//          | STATEMENT_EXPR
//          | STATEMENT_SELECTION
//          | STATEMENT_ITERATION
//          | STATEMENT_JUMP'

private:
    
    NodePtr next_statement;
    
protected:

    statement(NodePtr _arg1)
        :next_statement(_arg1)
    {}

public:
        std::string name = "statement";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
           next_statement->PrettyPrint(dst);
    
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }


};




class statement_labeled : public Node{

//STATEMENT_LABELED : IDENTIFIER ':' STATEMENT
//                  | CASE EXPR_CONST ':' STATEMENT
//                  | DEFAULT ':' STATEMENT
                  
private:

    NodePtr statement;
    std::string labels;
    NodePtr const_expr;



protected: 
    statement_labeled(std::string symbol,NodePtr _arg1)
        :statement(_arg1)
        ,labels(symbol)
        ,const_expr(NULL)
    {}
    
    statement_labeled(std::string symbol,NodePtr _arg1,NodePtr_arg2)
        :statement(_arg2)
        ,labels(symbol)
        ,const_expr(_arg1)
    {}
    

public:
    
        std::string name = "statement_labeled";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(const_expr != NULL){
                labels->PrettyPrint(dst);
                const_expr->PrettyPrint(dst);
                dst<<':';    
                statement->PrettyPrint(dst);
            }
            else{
                labels->PrettyPrint(dst);
                dst<<':';    
                statement->PrettyPrint(dst);     
            }           
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};




class statement_jump : public Node{
    
//STATEMENT_JUMP : GOTO IDENTIFIER ';'
//               | CONTINUE ';'
//               | BREAK ';'
//               | RETURN ';'
//               | RETURN EXPR ';'
private:
    NodePtr expr;
    std::string symbol;
    std::string symbol_2;
protected:
    
    statement_jump(std::string name_1,std::string name_2)
        :symbol(name_1)
        ,symbol_2(name_2)
    {}
    
    statement_jump(std::string name_1)
        :symbol(name_1)
        ,symbol_2(NULL)
    {}    
    
    statement_jump(std::string name_1,NodePtr _arg)
        :symbol(name_1)
        ,symbol_2(NULL)
        ,expr(_arg)
    {}
public:

        std::string name = "statement_jump";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(expr != NULL){
                symbol->PrettyPrint(dst);
                expr->PrettyPrint(dst);
                dst<<';';    
            }
            else if(symbol_2 != NULL){
                symbol->PrettyPrint(dst);
                symbol_2->PrettyPrint(dst);  
                dst<<';';        
            }           
            else{
                symbol->PrettyPrint(dst);
                dst<<';';
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};


class statement_iteration : public Node{
//STATEMENT_ITERATION : WHILE L_BRACKET EXPR R_BRACKET STATEMENT
//                    | DO STATEMENT WHILE L_BRACKET EXPR R_BRACKET ';' 
//                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR R_BRACKET STATEMENT
//                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR EXPR R_BRACKET STATEMENT
private:
    
    NodePtr expr;
    NodePtr statement;
    NodePtr statement_expr;
    std::string symbol;
    std::string symbol_1;
    NodePtr statement_expr_rep;
protected:
    statement_iteration(std::string name,NodePtr _arg1,NodePtr _arg2)
        :symbol(name)
        ,expr(_arg1)
        ,statement(_arg2)
        ,symbol_1(NULL)
        ,statement_expr(NULL)
        ,statement_expr_rep(NULL)
    {}
    statement_iteration(std::string name,NodePtr _arg1,std::string name_1,NodePtr _arg2)
        :symbol(name)
        ,expr(_arg2)
        ,statement(_arg1)
        ,symbol_1(name_1)
        ,statement_expr(NULL)
        ,statement_expr_rep(NULL)
    {}
    statement_iteration(std::string name,NodePtr _arg,NodePtr _arg1,NodePtr _arg2)
        :symbol(name)
        ,expr(NULL)
        ,statement(_arg2)
        ,symbol_1(NULL)
        ,statement_expr(_arg0)
        ,statement_expr_rep(arg1)

    {} 
    statement_iteration(std::string name,NodePtr _arg,NodePtr _arg1,NodePtr _arg2,NodePtr _arg3)
        :symbol(name)
        ,expr(_arg3)
        ,statement(_arg2)
        ,symbol_1(NULL)
        ,statement_expr(_arg0)
        ,statement_expr_rep(arg1)
    {}
public:
        std::string name = "statement_iteration";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(symbol_1!= NULL){
                symbol->PrettyPrint(dst);
                statement->PrettyPrint(dst);
                symbol_1->PrettyPrint(dst);
                expr->PrettyPrint(dst);
                dst<<';';
            }
            else if(expr == NULL){
                symbol->PrettyPrint(dst);
                statement_expr->PrettyPrint(dst);
                statement_expr_rep->PrettyPrint(dst);
                statement->PrettyPrint(dst);
                dst<';';
            }           
            else if(symbol_1 == NULL && statement_expr == NULL) {
                symbol->PrettyPrint(dst);
                dst<<'(';
                expr->PrettyPrint(dst);
                dst<<')'<<';';
            }
            else{
                symbol->PrettyPrint(dst);
                dst<<'(';
                statement_expr->PrettyPrint(dst);
                statement_expr_rep->PrettyPrint(dst);
                expr->PrettyPrint(dst);
                dst<<')';
                statement->PrettyPrint(dst);
            }
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};

class statement_selection : public Node{

//STATEMENT_SELECTION : IF L_BRACKET EXPR R_BRACKET STATEMENT
//                    | IF L_BRACKET EXPR R_BRACKET STATEMENT ELSE STATEMENT
//                    | SWITCH L_BRACKET EXPR R_BRACKET STATEMENT


private:

    NodePtr expr;
    NodePtr statement;
    NodePtr statement_rep;
    std::string symbol_1;
    std::string symbol_2;
    
    

protected:
    
    statement_selection(std::string name,NodePtr _arg,NodePtr _arg1)
        :expr(_arg)
        ,statement(_arg1)
        ,symbol_1(name)
        ,statement_rep(NULL)
        ,symbol_2(NULL)
    {}
    
    statement_selection(std::string name,NodePtr _arg,NodePtr _arg1,std::string name1 , NodePtr _arg2)
        :expr(_arg)
        ,statement(_arg1)
        ,symbol_1(name)
        ,statement_rep(_arg2)
        ,symbol_2(name1)
    {}



public:

    std::string name = "statement_selection";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(symbol_2 == NULL){
                symbol_1->PrettyPrint(dst);
                dst<<'(';
                expr->PrettyPrint(dst);
                dst<<')';
                statement->PrettyPrint(dst);
            }
            else{
                symbol_1->PrettyPrint(dst);
                dst<<'(';
                expr->PrettyPrint(dst);
                dst<<')';
                statement->PrettyPrint(dst);
                symbol_2->PrettyPrint(dst);
                statement_rep--PrettyPrint(dst);
            }           
  
        }

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};





#endif