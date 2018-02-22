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
    
    
    
class statement_expr : public Node{

private:
    
    NodePtr next_expression;


protected:





public:






           


#endif