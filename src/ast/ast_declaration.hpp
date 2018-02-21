#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_node.hpp"


class declaration_external : public Node {
    
    
private:

    NodePtr declaration_external;
    NodePtr definition_function;
    NodePtr declaration;


protected:
    
    declaration_external(NodePtr _arg1)
        :declaration_external(NULL)
        ,definition_function(_arg1)
        ,declaration(NULL)
    {}


    declaration_external(NodePtr _arg2)
        :declaration_external(NULL)
        ,definition_function(NULL)
        ,declaration(_arg2)
    {}
    


public:


    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->print();
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }

    virtual ~declaration_external(){}





class definition_function : public Node{



private:

    NodePtr  specifier_declaration;
    NodePtr  declarator;
    NodePtr  list_declaration;
    NodePtr  statement_compound;


protected:
    

    definition_function(NodePtr _arg1,NodePtr _arg2,NodePtr_arg3,NodePtr _arg4)
        :specifier_declaration(_arg1)
        ,declarator(_arg2)
        ,list_declaration(_arg3)
        ,statement_compound(_arg4)
        {}


    definition_function(NodePtr _arg1,NodePtr _arg2,NodePtr_arg3)
        :specifier_declaration(_arg1)
        ,declarator(_arg2)
        ,list_declaration(NULL)
        ,statement_compound(_arg3)
        {}



    definition_function(NodePtr _arg1,NodePtr _arg2,NodePtr _arg3)
        :specifier_declaration(NULL)
        ,declarator(_arg1)
        ,list_declaration(_arg2)
        ,statement_compound(_arg3)
        {}
 
 
 
 
    definition_function(NodePtr _arg1,NodePtr _arg2)
        :specifier_declaration(NULL)
        ,declarator(_arg1)
        ,list_declaration(NULL)
        ,statement_compound(_arg2)
        {}
 



public:


    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->print();
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }

    virtual ~declaration_external(){}








#endif