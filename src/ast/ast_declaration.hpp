#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_node.hpp"
#include "ast_statement.hpp"


class declaration_external : public Node {
    private:
        NodePtr arg; // could either be a function defition or a declaration - cannot tell

    public:    
        declaration_external(NodePtr _arg)
            : arg(_arg)
        {}

        std::string name = "declaration_external"; 

        virtual void PrettyPrint(std::ostream &dst) const override{
        
         /*
            if(next != null) next->print();
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }   
};

class definition_function : public Node{


    //DEFINITION_FUNCTION : SPECIFIER_DECLARATION DECLARATOR LIST_DECLARATION STATEMENT_COMPOUND { $$ = new definition_function($1, $2, $3, $4); }
    //                    | SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND                  { $$ = new definition_function($1, $2, '$', $3); }
    //                    | DECLARATOR LIST_DECLARATION STATEMENT_COMPOUND                       { $$ = new definition_function('$', $1, $2, $3); }
    //                    | DECLARATOR STATEMENT_COMPOUND       

    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator;
        NodePtr  list_declaration;
        NodePtr  statement_compound;

    public:
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

        std::string name = "definition_function"; 

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }
        
        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
           //         | SPECIFIER_DECLARATION DECLARATOR STATEMENT_COMPOUND                  { $$ = new definition_function($1, $2, '$', $3); }
            context.function_dec = true;
            declarator->translate(dst,context); 
            context.indent++;
           
            std::vector<std::string>::iterator myVector;
            
            for(myVector = context.globalVar.begin() ; myVector != context.globalVar.end() ; myVector++){
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"global "<<*myVector<<std::endl;    
            }     
                
            statement_compound->translate(dst,context);
            context.indent--;
            context.function_dec = false;

        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
        declarator->compile(dst,context);
        // presevre return address
        dst<<"\taddiu\t$sp,$sp,-4\n"; // pushed stack down
        context.stack_size++;
        dst<<"\tsw\t$fp,4($sp)\n"; // stores value of fp intp sp+4
        dst<<"\taddu\t$fp,$sp,$0\n"; // moves value of sp into fp

        // now evaluate compound statement

        statement_compound->compile(dst, context);

        dst<<'\t'<<".end "<<context.current_func<<std::endl;
        
        }
};

class declaration : public Node{

    //DECLARATION : SPECIFIER_DECLARATION ';'                       { $$ = new declaration($1); }
    //            | SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'  { $$ = new declaration($1, $2); }

    private:
        NodePtr  specifier_declaration;
        NodePtr  declarator_list_init;
        
    public:
        declaration(NodePtr _arg1)
            :specifier_declaration(_arg1)
            ,declarator_list_init(NULL)
        {}
        
        declaration(NodePtr _arg1,NodePtr _arg2) 
            :specifier_declaration(_arg1)
            ,declarator_list_init(_arg2)
        {}

        std::string name = "declaration"; 

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) {next->print();
                // SPECIFIER_DECLARATION DECLARATOR_INIT_LIST ';'
            }
            // SPECIFIER_DECLARATION ';'
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            //specifier_declaration->translate(dst,context);
           // dst<<" ";
            declarator_list_init->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(declarator_list_init != NULL){
                
                binding temp;
                
                specifier_declaration->compile(dst,context); // assigns tmp_v with C type
                temp.type = context.tmp_v;
                context.set_expr_result_type(); // use for assign cases on type
                temp.reg_ID = 33;    // forgot to initialise reg_ID into 33 -> not empty
                
                this->push_stack(dst,context); //stack size is changed here.(incremented)
                temp.stack_position = context.stack_size;

                declarator_list_init->compile(dst,context); // Returns Identifier of variable to temp_v
                
                
                context.scopes[context.scope_index][context.tmp_v] = temp; // not sure if this map works
                
                                
                                
           
            }
              

                
            


        }
        
        
};

class specifier_declaration : public Node{
    private:
        NodePtr present;
        NodePtr next;
        
    public:
        specifier_declaration(NodePtr _arg1)
            :present(_arg1)
            ,next(NULL)
        {}
        specifier_declaration(NodePtr _arg1,NodePtr _arg2)
            :present(_arg1)
            ,next(_arg2)
        {}    
    public:
        std::string name = "specifier_declaration";   

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*        

            if(next != null) next->print();
            declaration->print();
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            present->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            present->compile(dst,context);
            
            if(context.tmp_v == "unsigned"){
                
                next->compile(dst,context);
                
                context.tmp_v = "unsigned " + context.tmp_v;
            }
            
            
            
            
            
            
            
            
        
        
        }
};

class list_declaration : public Node{
    //LIST_DECLARATION : DECLARATION
    //               | LIST_DECLARATION DECLARATION    
    
    private:  
        NodePtr declaration;
        NodePtr list_declaration1;
        
    public:   
        list_declaration(NodePtr _arg1)
            :declaration(_arg1)
            ,list_declaration1(NULL)
        {}
            
        list_declaration(NodePtr _arg1,NodePtr _arg2)
            :declaration(_arg2)
            ,list_declaration1(_arg1)
        {}

        std::string name = "list_declaration";              
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            list_declaration1->translate(dst,context);
            declaration->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            
            list_declaration1->compile(dst,context);
            
            declaration->compile(dst,context);
            
            
            

        }
};

class declarator_init_list : public Node{
    //init_declarator_list
    //  : init_declarator
    //  | init_declarator_list ',' init_declarator
    //  ;

    private:
        NodePtr declarator_init1;
        NodePtr declarator_init;

    public:    
        declarator_init_list(NodePtr _arg1)
            :declarator_init1(_arg1)
            ,declarator_init(NULL)
        {}
        
        declarator_init_list(NodePtr _arg1,NodePtr _arg2)
            :declarator_init1(_arg2)
            ,declarator_init(_arg1)
        {}
    
    public:

        std::string name = "declarator_init_list"; 

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->PrettyPrintp();
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(declarator_init != NULL){
                declarator_init->translate(dst,context);

            }
                declarator_init1->translate(dst,context);            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {

            declarator_init->compile(dst,context);
            
            declarator_init1->compile(dst,context);
            


        }
};

class declarator_init : public Node{

    //DECLARATOR_INIT : DECLARATOR                        { $$ = new declarator_init_list($1);    }
    //                | DECLARATOR ASSIGN INITIALIZER     { $$ = declarator_init( $1,"=" , $3);}

    private:
        NodePtr declarator;
        NodePtr initializer;
        char symbol;
        
    public:
    
        declarator_init(NodePtr _arg1) 
            :declarator(_arg1)
            ,initializer(NULL)
            ,symbol(' ')
        {}
    
        declarator_init(NodePtr _arg1,char name,NodePtr _arg2)
            :declarator(_arg1)
            ,initializer(_arg2)
            ,symbol(name)
        {}
        
        
    public:
        
        std::string name = "declarator_init";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }
        
        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            
            declarator->translate(dst,context);
            
            
            if(!context.function_dec){    //this checks if it is a global variable, which it is
                context.globalVar.push_back(context.tmp_v);
            }

            if(symbol == '='){
                dst<<"=";
                initializer->translate(dst,context);
                dst<<"\n";
            }
            else{
                dst<<"=0\n";        // initial 
            }
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            
            declarator->compile(dst,context);   //stores into tmp_V (variable name)
            
            if( initializer != NULL){
                context.assigning = true;
                initializer->compile(dst,context); //if its a constant it stores into expression_results
                dst<<"\tadd\t$15,$0,"<<context.expr_result<<'\n';
                context.assigning = false;
            }
            else{
                dst<<"\tmove\t$15,$0\n";
            }

            dst<<"\tsw\t$15,"<<context.stack_size*4<<"($fp)"<<std::endl; //stores the value onto the correct position on the stack.
            
        }
};

class initializer : public Node{
    //INITIALIZER : EXPR_ASSIGNMENT    
    //           | L_BRACKET LIST_INITIALIZER R_BRACKET
    //           | L_BRACKET LIST_INITIALIZER ',' R_BRACKET 
            
    private:
        NodePtr list_initializer;

    public: 
        initializer(NodePtr _arg1) 
            : list_initializer(_arg1)
        {}

        std::string name = "initializer";
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class list_initializer : public Node{
    //LIST_INITIALIZER : INITIALIZER
    //                | LIST_INITIALIZER ',' INITIALIZER
                    
    private:
        NodePtr initializer ;
        NodePtr rec;

    public:
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

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERE
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declarator_abstract : public Node{
    //DECLARATOR_ABSTRACT : POINTER
    //                    | DECLARATOR_DIRECT_ABSTRACT
    //                    | POINTER DECLARATOR_DIRECT_ABSTRACT

    private:    
        NodePtr pointer;
        NodePtr declarator_direct_abstract;
        
    public:
        declarator_abstract(NodePtr _arg1)
            :pointer(_arg1)
            ,declarator_direct_abstract(NULL)
        {}
        
        
        declarator_abstract(NodePtr _arg1,NodePtr _arg2)
            :pointer(_arg1)
            ,declarator_direct_abstract(_arg2)
        {}
        
    public:
        std::string name = "declarator_abstract";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
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
        
    public:
    // LOOK AT THE DIFFERENTIATION LATER
    /*
            declarator_direct_abstract(NodePtr _arg1,char x)
                :now(_arg1)
                ,next(NULL)
            // ,bracket(true)
            {}
            */
            
            declarator_direct_abstract()
                :now(NULL)
                ,next(NULL)
            // ,bracket(false)
            {}
            
            declarator_direct_abstract(NodePtr _arg1)
                :now(_arg1)
                ,next(NULL)
            //  ,bracket(false)
            {}
    /*
        declarator_direct_abstract(NodePtr _arg1,NodePtr _arg2,char x)
            :now(_arg1)
            ,next(_arg2)
         //   ,bracket(false)
        {}
        */
        /*
        declarator_direct_abstract(char x)
            :now(NULL)
            ,next(NULL)
          //  ,bracket(true)
        {}   
        */
        declarator_direct_abstract(NodePtr _arg1,NodePtr _arg2)
            :now(_arg1)
            ,next(_arg2)
           // ,bracket(true)
        {} 

        std::string name = "declarator_direct_abstract";
        
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declaration_parameter : public Node{


    //DECLARATION_PARAMETER : SPECIFIER_DECLARATION DECLARATOR        
    //                      | SPECIFIER_DECLARATION DECLARATOR_ABSTRACT
    //                      | SPECIFIER_DECLARATION
    private:
        NodePtr current;
        NodePtr recur;

    public:
        declaration_parameter(NodePtr _arg1, NodePtr _arg2)
            :current(_arg1)
            ,recur(_arg2)
        {}
        
        declaration_parameter(NodePtr _arg1)
            :current(_arg1)
            ,recur(NULL)
        {}

        std::string name = "declaration_parameter";
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(recur != NULL) recur->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declarator_direct : public Node{

    //          DECLARATOR_DIRECT : IDENTIFIER
    //                | L_BRACKET DECLARATOR R_BRACKET                              (declarator)
    //                | DECLARATOR_DIRECT L_SQUARE EXPR_CONST R_SQUARE              declarator_direct [exprconst]
    //                | DECLARATOR_DIRECT L_SQUARE R_SQUARE                         declarator_direct []
    //                | DECLARATOR_DIRECT L_BRACKET LIST_PARAM_TYPE R_BRACKET       declarator_direct (list_param_type)
    //                | DECLARATOR_DIRECT L_BRACKET LIST_IDENTIFIER R_BRACKET       declarator_direct (list_identifier)
    //                | DECLARATOR_DIRECT L_BRACKET R_BRACKET                       declarator_direct ()
    private:
        NodePtr  current;
        NodePtr  next;
        bool brackets;
        bool squares;
        std::string symbol;
        
    public:
        declarator_direct(std::string name)
            :current(NULL)
            ,next(NULL)
            ,brackets(false) //MEHEDI really wants false
            ,squares(false)
            ,symbol(name)
        {}
        
        declarator_direct(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
            ,brackets(true)
            ,squares(false)
            ,symbol(" ")
        {}
        
        declarator_direct(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
            ,brackets(true) // fix this for the actual thing
            ,squares(false)
            ,symbol(" ")
        {}

        std::string name = "declarator_direct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(symbol != " "){
                
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }            
                dst<<symbol;
                context.tmp_v = symbol;
                
                if(symbol == "main"){
                    context.main_exists = true;
                }
            }          
            else{

                
                    dst<<"def ";

                    current->translate(dst,context);
                    dst<<"(";
                    if(next != NULL){
                        next->translate(dst,context);
                    }  
                    dst<<")"<<":"<<std::endl;  
                
            }
            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            if(symbol != " "){
    
                if(context.scope_index == 0){
                    dst<<'\t'<<".global "<<symbol<<'\n';
                    context.current_func = symbol;
                    
                }
                
                context.tmp_v = symbol;                    
                    
            }
            
            else if(brackets){
                    current->compile(dst,context);
                    dst<<'\t'<<".ent "<<context.current_func<<'\n';
                    dst<<"main:"<<'\n';
            }
            
            //HEY 
        }

};

class struct_declarator : public Node{
    //STRUCT_DECLARATOR : DECLARATOR
    //                | ':' EXPR_CONST
    //                | DECLARATOR ':' EXPR_CONST

    private:
        NodePtr current;
        NodePtr next;

    public:
        struct_declarator(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
        {}

        struct_declarator(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}

        std::string name = "struct_declarator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        } 
};

class list_struct_declarator : public Node{
    //LIST_STRUCT_DECLARATOR : STRUCT_DECLARATOR
    //                       | LIST_STRUCT_DECLARATOR ',' STRUCT_DECLARATOR

    private:
        NodePtr current;
        NodePtr next;

    public:
        list_struct_declarator(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
        {}

        list_struct_declarator(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}
        
        
        std::string name = "list_struct_declarator";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */ 
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declaration_struct : public Node{
    //DECLARATION_STRUCT : LIST_SPEC_QUAL LIST_STRUCT_DECLARATOR ';'

    private:
        NodePtr current;
        NodePtr next;
        
    public:
        declaration_struct(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}

        std::string name = "declaration_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */ 
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declaration_list_struct : public Node{
    //DECLARATION_LIST_STRUCT : DECLARATION_STRUCT
    //                        | DECLARATION_LIST_STRUCT DECLARATION_STRUCT

    private:
        NodePtr current;
        NodePtr rec;

    public:
        declaration_list_struct(NodePtr _arg1)
            :current(_arg1)
            ,rec(NULL)
        {}
        
        declaration_list_struct(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,rec(_arg2)
        {}

        std::string name = "declaration_list_struct";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*

            if(next != null) next->print();
            declaration->PrettyPrintp();
            //MEHEDI IS TRIGGERED 
            dst<<'(';
        */  
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class declarator : public Node{
    //DECLARATOR : POINTER DECLARATOR_DIRECT
    //           | DECLARATOR_DIRECT  
    private:
        NodePtr ptr;
        NodePtr dd;
    public:
        declarator(NodePtr _arg1, NodePtr _arg2)
            : ptr(_arg1)
            , dd(_arg2)
        {}

    public:

        std::string name = "declarator";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            ptr->PrettyPrint(dst);
            dd->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support transalte function"<<std::endl;
            exit(1);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;

            exit(1);
        }
};


#endif



