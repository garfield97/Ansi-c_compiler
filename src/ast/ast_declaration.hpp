#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_node.hpp"
#include "ast_statement.hpp"


class declaration_external : public Node {
    private:

        NodePtr arg; // could either be a function defition or a declaration - cannot tell

    protected:
        
        declaration_external(NodePtr _arg)
            : arg(_arg)
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
};


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

        definition_function(NodePtr _arg1,NodePtr _arg2, char x, NodePtr_arg3) // char used to differentiate
            :specifier_declaration(_arg1)
            ,declarator(_arg2)
            ,list_declaration(NULL)
            ,statement_compound(_arg3)
        {}

        definition_function(char x, NodePtr _arg_1,NodePtr _arg_2,NodePtr _arg_3) // char used to differentiate
            :specifier_declaration(NULL)
            ,declarator(_arg_1)
            ,list_declaration(_arg_2)
            ,statement_compound(_arg_3)
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
};


class declaration : public Node{
    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator_list_init;
        
    protected:      
        declaration(NodePtr _arg1)
            :specifier_declaration
            ,declarator_list_init(NULL)
        {}
        
        declaration(NodePtr _arg1,NodePtr _arg2) 
            :specifier_declaration(_arg1)
            ,declarator_list_init(_arg2)
        {}

    public:
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(next != null) {next->print();
                // SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'
            }
            // SPECIFIER_DECLARATION ';'
            declaration->print();
        }

        virtual void toPYPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
};



class specifier_declaration : public Node{
    private:
        NodePtr present;
        NodePtr next;
        
    protected:
        specifier_declaration(NodePtr _arg1)
            :present(_arg1)
            ,next(NULL)
        {}
        specifier_declaration(NodePtr _arg1,NodePtr _arg2)
            :present(_arg1)
            ,next(_arg2)
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
};



class list_declaration : public Node{
//LIST_DECLARATION : DECLARATION
  //               | LIST_DECLARATION DECLARATION    
    
private:  
    
    NodePtr declaration;
    NodePtr list_declaration;
    
protected:
        
    list_declaration(NodePtr _arg1)
        :declaration(_arg1)
        ,list_declaration(NULL)
    {}
        
    
    list_declaration(NodePtr _arg1,NodePtr _arg2)
        :declaration(_arg2)
        ,list_declaration(_arg1)
    {}
              
    
public:


    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERE
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }
};



class declarator_init_list : public Node{


//init_declarator_list
//  : init_declarator
//  | init_declarator_list ',' init_declarator
//  ;


private:

    NodePtr declarator_init_list;
    NodePtr declarator_init;


protected:
    
    declarator_init_list(NodePtr _arg1)
        :declarator_init_list(_arg1)
        ,declarator_init(NULL)
    {}
    
    declarator_init_list(NodePtr _arg1,NodePtr _arg2)
        :declarator_init_list(_arg1)
        ,declarator_init(_arg2)
    {}
    


public:

    std::string name = "declarator_init_list"; 

    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERE
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }


};


class declarator_init : public Node{

private:
    
    NodePtr declarator;
    NodePtr initializer;
    
protected:
    
    declarator_init(NodePtr _arg1) 
        :declarator(_arg1)
    {}
    
    declarator_init(NodePtr _arg1,NodePtr _arg2)
        :declarator(_arg1)
        ,initializer(_arg2)
    {}
    
public:
    
    std::string name = "declarator_init"

    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERE
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }


};


class initializer : public Node{
//INITIALIZER : EXPR_ASSIGNMENT    
//           | L_BRACKET LIST_INITIALIZER R_BRACKET
//           | L_BRACKET LIST_INITIALIZER ',' R_BRACKET 
            
private:
    NodePtr list_initializer;

protected:
    
    initializer(NodePtr _arg1) 
        : list_initializer(_arg1)
    {}


public:


    std::string name = "initializer"

    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERE
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }




};


class list_initializer : public Node{
//LIST_INITIALIZER : INITIALIZER
 //                | LIST_INITIALIZER ',' INITIALIZER
                 

private:
    NodePtr initializer 
    NodePtr list_initializer

protected:

    list_initializer(NodePtr _arg1)
        :initializer(_arg1)
        ,list_initializer(NULL)
    {}
    

    list_initializer(NodePtr _arg1,NodePtr _arg2)
        :initializer(_arg1)
        ,list_initializer(_arg2)
    {}
    


public:

    std::string name = "list_initializer"

    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERE
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }



};

class declarator_abstract : public Node{
    

//DECLARATOR_ABSTRACT : POINTER
//                    | DECLARATOR_DIRECT_ABSTRACT
//                    | POINTER DECLARATOR_DIRECT_ABSTRACT

private:
    
    NodePtr pointer
    NodePtr declarator_direct_abstract
    

protected:

    declarator_abstract(NodePtr _arg1)
        :pointer(_arg1)
        ,declarator_direct_abstract(NULL)
    {}
    
    
    declarator_abstract(NodePtr _arg1,NodePtr _arg2)
        :pointer(_arg1)
        ,declarator_direct_abstract(_arg2)
    {}
    
public:


    std::string name = "declarator_abstract"

    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERED 
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }



};


class declarator_direct_abstract : public Node{

//                   DECLARATOR_DIRECT_ABSTRACT : L_BRACKET DECLARATOR_ABSTRACT R_BRACKET
//                           | L_SQUARE R_SQUARE
//                           | L_SQUARE EXPR_CONST R_SQUARE
//                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE R_SQUARE
//                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE EXPR_CONST R_SQUARE
//                           | L_BRACKET R_BRACKET
//                           | L_BRACKET LIST_PARAM_TYPE R_BRACKET
//                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET R_BRACKET
//                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET LIST_PARAM_TYPE R_BRACKET

private:
    NodePtr pointer
    NodePtr declarator_direct_abstract
protected:

    declarator_abstract(NodePtr _arg1)
        :pointer(_arg1)
        ,declarator_direct_abstract(NULL)
    {}
    
    
    declarator_abstract(NodePtr _arg1,NodePtr _arg2)
        :pointer(_arg1)
        ,declarator_direct_abstract(_arg2)
    {}
    
public:


    std::string name = "declarator_abstract"

    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->PrettyPrintp();
        //MEHEDI IS TRIGGERED 
        dst<<'(';
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }



};








#endif