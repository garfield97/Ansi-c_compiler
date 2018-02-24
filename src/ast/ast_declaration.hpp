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

/*
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(next != null) next->print();
            declaration->print();
        }

        virtual void toPYPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
*/
};


class definition_function : public Node{
    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator;
        NodePtr  list_declaration;
        NodePtr  statement_compound;

    protected:
        definition_function(NodePtr _arg1,NodePtr _arg2,NodePtr _arg3,NodePtr _arg4)
            :specifier_declaration(_arg1)
            ,declarator(_arg2)
            ,list_declaration(_arg3)
            ,statement_compound(_arg4)
        {}

        definition_function(NodePtr _arg1,NodePtr _arg2, char x, NodePtr _arg3) // char used to differentiate
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
/*
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(next != null) next->print();
            declaration->print();
        }

        virtual void toPYPY(std::ostream &dst) const override{

        }

        virtual void renderASM(std::ostream &dst) const override{

        }
*/
};


class declaration : public Node{
    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator_list_init;
        
    protected:      
        declaration(NodePtr _arg1)
            :specifier_declaration(_arg1)
            ,declarator_list_init(NULL)
        {}
        
        declaration(NodePtr _arg1,NodePtr _arg2) 
            :specifier_declaration(_arg1)
            ,declarator_list_init(_arg2)
        {}

    public:

/*
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
*/
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
/*        
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
*/
};



class list_declaration : public Node{
//LIST_DECLARATION : DECLARATION
  //               | LIST_DECLARATION DECLARATION    
    
private:  
    
    NodePtr declaration;
    NodePtr list_declaration1;
    
protected:
        
    list_declaration(NodePtr _arg1)
        :declaration(_arg1)
        ,list_declaration1(NULL)
    {}
        
    
    list_declaration(NodePtr _arg1,NodePtr _arg2)
        :declaration(_arg2)
        ,list_declaration1(_arg1)
    {}
              
    
public:

/*
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
*/
};



class declarator_init_list : public Node{


//init_declarator_list
//  : init_declarator
//  | init_declarator_list ',' init_declarator
//  ;


private:

    NodePtr declarator_init1;
    NodePtr declarator_init;


protected:
    
    declarator_init_list(NodePtr _arg1)
        :declarator_init1(_arg1)
        ,declarator_init(NULL)
    {}
    
    declarator_init_list(NodePtr _arg1,NodePtr _arg2)
        :declarator_init1(_arg1)
        ,declarator_init(_arg2)
    {}
    


public:

    std::string name = "declarator_init_list"; 
/*
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
*/

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
    
    std::string name = "declarator_init";
/*
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
*/


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


    std::string name = "initializer";
/*
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



*/
};


class list_initializer : public Node{
//LIST_INITIALIZER : INITIALIZER
 //                | LIST_INITIALIZER ',' INITIALIZER
                 

private:
    NodePtr initializer ;
    NodePtr rec;

protected:

    list_initializer(NodePtr _arg1)
        :initializer(_arg1)
        ,rec(NULL)
    {}
    

    list_initializer(NodePtr _arg1,NodePtr _arg2)
        :initializer(_arg1)
        ,rec(_arg2)
    {}
    


public:

    std::string name = "list_initializer";
/*
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

*/
};

class declarator_abstract : public Node{
    

//DECLARATOR_ABSTRACT : POINTER
//                    | DECLARATOR_DIRECT_ABSTRACT
//                    | POINTER DECLARATOR_DIRECT_ABSTRACT

private:
    
    NodePtr pointer;
    NodePtr declarator_direct_abstract;
    

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

/*
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

*/

};


class declarator_direct_abstract : public Node{

//                   DECLARATOR_DIRECT_ABSTRACT : L_BRACKET DECLARATOR_ABSTRACT R_BRACKET           1 pointer needed
//                           | L_SQUARE R_SQUARE                                                         0 needed
//                           | L_SQUARE EXPR_CONST R_SQUARE                                       1 pointer needed
//                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE R_SQUARE                         1 pointer
//                           | DECLARATOR_DIRECT_ABSTRACT L_SQUARE EXPR_CONST R_SQUARE              2 pointer
//                           | L_BRACKET R_BRACKET                                                      0
//                           | L_BRACKET LIST_PARAM_TYPE R_BRACKET                                      1 
//                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET R_BRACKET                                   1
//                           | DECLARATOR_DIRECT_ABSTRACT L_BRACKET LIST_PARAM_TYPE R_BRACKET                       2 pointer

private:
    NodePtr now;
    NodePtr next;
    bool bracket;
    
protected:

// LOOK AT THE DIFFERENTIATION LATER

    declarator_direct_abstract(NodePtr _arg1,char x)
        :now(_arg1)
        ,next(NULL)
        ,bracket(true)
    {}
    
    
    declarator_direct_abstract()
        :now(NULL)
        ,next(NULL)
        ,bracket(false)
    {}
    
    declarator_direct_abstract(NodePtr _arg1)
        :now(_arg1)
        ,next(NULL)
        ,bracket(false)
    {}

    declarator_direct_abstract(NodePtr _arg1,NodePtr _arg2,char x)
        :now(_arg1)
        ,next(_arg2)
        ,bracket(false)
    {}
    
    declarator_direct_abstract(char x)
        :now(NULL)
        ,next(NULL)
        ,bracket(true)
    {}   
    
    declarator_direct_abstract(NodePtr _arg1,NodePtr _arg2)
        :now(_arg1)
        ,next(_arg2)
        ,bracket(true)
    {} 
public:


    std::string name = "declarator_direct_abstract";
/*
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
*/



};


class declaration_parameter : public Node{


//DECLARATION_PARAMETER : SPECIFIER_DECLARATION DECLARATOR        
//                    | SPECIFIER_DECLARATION DECLARATOR_ABSTRACT
//                    | SPECIFIER_DECLARATION
private:
    
    NodePtr current;
    NodePtr recur;


protected:

    declaration_parameter(NodePtr _arg1, NodePtr _arg2)
        :current(_arg1)
        ,recur(_arg2)
    {}
    
    declaration_parameter(NodePtr _arg1)
        :current(_arg1)
        ,recur(NULL)
    {}

public:

    std::string name = "declaration_parameter";
/*
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
*/
};


class declarator_direct : public Node{

//          DECLARATOR_DIRECT : IDENTIFIER
//                | L_BRACKET DECLARATOR R_BRACKET                              (declarator)
//                | DECLARATOR_DIRECT L_SQUARE EXPR_CONST R_SQUARE              declarator_direct [exprconst]
//                | DECLARATOR_DIRECT L_SQUARE R_SQUARE                         declarator_direct []
//                | DECLARATOR_DIRECT L_BRACKET LIST_PARAM_TYPE R_BRACKET       declarator_direct (list_param_type)
//                | DECLARATOR_DIRECT L_BRACKET LIST_IDENTIFIER R_BRACKET       declarator_direct (list_identifier)
//                | DECLARATOR_DIRECT L_BRACKET R_BRACKET                       declarator ()
private:

    NodePtr  current;
    NodePtr  next;
    bool brackets;
    std::string symbol;


protected:

    declarator_direct(std::string name)
        :symbol(name)
        ,current(NULL)
        ,next(NULL)
        ,brackets(false) //MEHEDI really wants false
    {}
    
    declarator_direct(NodePtr _arg1,NodePtr _arg2)
        :symbol(NULL)
        ,current(_arg1)
        ,next(_arg2)
        ,brackets(false)
    {}
     
    declarator_direct(NodePtr _arg1)
        :symbol(NULL)
        ,current(_arg1)
        ,next(NULL)
        ,brackets(false)    
    {}

public:

    std::string name = "declarator_direct";
/*
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

*/

};


class struct_declarator : public Node{

//STRUCT_DECLARATOR : DECLARATOR
//                | ':' EXPR_CONST
//                | DECLARATOR ':' EXPR_CONST

private:

    NodePtr current;
    NodePtr next;

protected:
    
    struct_declarator(NodePtr _arg1)
        :current(_arg1)
        ,next(NULL)
    {}

    struct_declarator(NodePtr _arg1,NodePtr _arg2)
        :current(_arg1)
        ,next(_arg2)
    {}

public:

    std::string name = "struct_declarator";
/*
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
*/

};


class list_struct_declarator : public Node{

//LIST_STRUCT_DECLARATOR : STRUCT_DECLARATOR
//                       | LIST_STRUCT_DECLARATOR ',' STRUCT_DECLARATOR
private:
    NodePtr current;
    NodePtr next;

protected:
    
    list_struct_declarator(NodePtr _arg1)
        :current(_arg1)
        ,next(NULL)
    {}

    list_struct_declarator(NodePtr _arg1,NodePtr _arg2)
        :current(_arg1)
        ,next(_arg2)
    {}
    
    
public:
/*
    std::string name = "list_struct_declarator"

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
*/ 




};




class declaration_struct : public Node{

//DECLARATION_STRUCT : LIST_SPEC_QUAL LIST_STRUCT_DECLARATOR ';'
private:

    NodePtr current;
    NodePtr next;
    
protected:
    
    declaration_struct(NodePtr _arg1,NodePtr _arg2)
        :current(_arg1)
        ,next(_arg2)
    {}

public:    
    
    std::string name = "declaration_struct";
/*
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
 */ 

};

class declaration_list_struct : public Node{

//DECLARATION_LIST_STRUCT : DECLARATION_STRUCT
//                        | DECLARATION_LIST_STRUCT DECLARATION_STRUCT
private:
    NodePtr current;
    NodePtr rec;

protected:

    declaration_list_struct(NodePtr _arg1)
        :current(_arg1)
        ,rec(NULL)
    {}

    
    declaration_list_struct(NodePtr _arg1,NodePtr _arg2)
        :current(_arg1)
        ,rec(_arg2)
    {}
    
    
public:

    std::string name = "declaration_list_struct";
/*
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
  */  

//not sure if delcaration is complete
//lets move onto int main recursion

};


#endif
