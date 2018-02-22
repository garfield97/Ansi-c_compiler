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

        virtual void toPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }






           


#endif