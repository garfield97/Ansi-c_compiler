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


    definition_function(NodePtr _arg1,NodePtr _arg2,NodePtr_arg3)
        :specifier_declaration(_arg1)
        ,declarator(_arg2)
        ,list_declaration(NULL)
        ,statement_compound(_arg3)
    {}



    definition_function(NodePtr _arg_1,NodePtr _arg_2,NodePtr _arg_3)
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
    char semicolon_in
    
protected:
    
    declaration(NodePtr _arg1,char semicolon)
        :specifier_declaration
        ,declarator_list_init(NULL)
        ,semicolon_in(semicolon)
    {}
    
    
    declaration(NodePtr _arg1,NodePtr _arg2,char semicolon) 
        :specifier_declaration(_arg1)
        ,declarator_list_init(_arg2)
        ,semicolon_in(semicolon)
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



class specifier_declaration : public Node{

private:
    
    NodePtr specifier_store_class;
    NodePtr specifier_declaration;
    NodePtr specifier_type;
    NodePtr qualifier_type;
    



protected:

//Not sure if the functional overloading actually works properly though
    specifier_declaration(NodePtr _arg1)
        :specifier_store_class(_arg1)
        ,specifier_declaration(NULL)
        ,specifier_type(NULL)
        ,qualifier_type(NULL)
    {}
    specifier_declaration(NodePtr _arg1,NodePtr _arg2)
        :specifier_store_class(_arg1)
        ,specifier_declaration(_arg2)
        ,specifier_type(NULL)
        ,qualifier_type(NULL)
    {}    
    
    specifier_declaration(NodePtr _arg_1)
        :specifier_store_class(NULL)
        ,specifier_declaration(NULL)
        ,specifier_type(_arg_1)
        ,qualifier_type(NULL)
    {}    
    specifier_declaration(NodePtr _arg_1,NodePtr _arg_2)
        :specifier_store_class(NULL)
        ,specifier_declaration(_arg_1)
        ,specifier_type(_arg_2)
        ,qualifier_type(NULL)
    {}
    
    specifier_declaration(NodePtr _arg1__)
        :specifier_store_class(NULL)
        ,specifier_declaration(NULL)
        ,specifier_type(NULL)
        ,qualifier_type(_arg1__)
    {}        
    specifier_declaration(NodePtr _arg1__,NodePtr _arg2__)
        :specifier_store_class(NULL)
        ,specifier_declaration(_arg2__)
        ,specifier_type(NULL)
        ,qualifier_type(_arg1__)
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
        declaration->print();
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }




#endif