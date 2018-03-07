#ifndef ast_statement_hpp
#define ast_statement_hpp

#include "ast_node.hpp"

class statement_compound : public Node{
    //STATEMENT_COMPOUND : L_BRACE R_BRACE                                  { $$ = new statement_compound(); }
    //                   | L_BRACE LIST_STATEMENT R_BRACE                   { $$ = new statement_compound($2); }
    //                   | L_BRACE LIST_DECLARATION R_BRACE                 { $$ = new statement_compound($2); }
    //                   | L_BRACE LIST_DECLARATION LIST_STATEMENT R_BRACE 
    private:
        NodePtr current;
        NodePtr next;

    public:
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
        
        std::string name = "statement_compound";
            
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*  
            
            if(rec != NULL){
                    rec->PrettyPrint(dst);
                    dst<<'^ ';
            }
            exp->PrettyPrint(dst);
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
      
           
            
            if(current == NULL && next == NULL){
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                dst<<"pass\n";
            }
            
            else{
                
                current->translate(dst,context);
            }
            
            if(next != NULL){
                
                next->translate(dst,context);
            }
         
            
            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            context.scope_index++; // incrementing the scope index
            std::map<std::string,binding>bindings; //create a map for pushing it out at the end.
            context.scopes.push_back(bindings);
            
            if(current != NULL){
                
                current->compile(dst,context);
            }
            
            if(next != NULL){
                
                next->compile(dst,context);
            }
        
        
            context.scope_index--; // decrementing the scope index so we can keep track of which scope we are in
            context.scopes.pop_back(); // pop out
            

        }
};

class list_statement : public Node{
    //LIST_STATEMENT : STATEMENT
    //               | LIST_STATEMENT STATEMENT         

    private:
        NodePtr current;
        NodePtr next;

    public:   
    /*
        list_statement(NodePtr _arg1)
            :current(_arg1)
            ,next(NULL)
        {}
        */
        list_statement(NodePtr _arg1,NodePtr _arg2)
            :current(_arg1)
            ,next(_arg2)
        {}

        std::string name = "list_statement";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            /*

            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'^ ';
            }
            exp->PrettyPrint(dst);
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);

        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            current->translate(dst,context);
            next->translate(dst,context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            current->compile(dst,context);
            next->compile(dst,context);
        }

};

class statement_expr : public Node{
    //STATEMENT_EXPR : ';'
    //               | EXPR ';'

    private:
        NodePtr current;

    public:
            //since first line its no non terminal matched, terminal symbol only so maybe empty set?
        
        statement_expr()
            :current(NULL)
        {}
        
        statement_expr(NodePtr _arg1)
            :current(_arg1)
        {}
 
        std::string name = "statement_expr";
   
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
            if(rec != NULL){
                rec->PrettyPrint(dst);
                dst<<'^ ';
            }
            exp->PrettyPrint(dst);
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
        
            for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
            }     
            if(current != NULL){
                current->translate(dst,context);
            }
            dst<<std::endl;
            
            
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};
        
class statement : public Node{
    //STATEMENT : STATEMENT_LABELED
    //          | STATEMENT_COMPOUND
    //          | STATEMENT_EXPR
    //          | STATEMENT_SELECTION
    //          | STATEMENT_ITERATION
    //          | STATEMENT_JUMP

    private:
        NodePtr next_statement;
        
    public:
    
    /*
        statement(NodePtr _arg1)
            :next_statement(_arg1)
        {}
    */
        std::string name = "statement";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
           next_statement->PrettyPrint(dst);
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

class statement_labeled :public Node{
    //STATEMENT_LABELED : IDENTIFIER ':' STATEMENT
    //                  | CASE EXPR_CONST ':' STATEMENT
    //                  | DEFAULT ':' STATEMENT
                  
    private:
        NodePtr statement;
        std::string labels;
        NodePtr const_expr;

    public: 
        statement_labeled(std::string symbol,NodePtr _arg1)
            :statement(_arg1)
            ,labels(symbol)
            ,const_expr(NULL)
        {}
        
        statement_labeled(std::string symbol,NodePtr _arg1,NodePtr _arg2)
            :statement(_arg2)
            ,labels(symbol)
            ,const_expr(_arg1)
        {} 

        std::string name = "statement_labeled";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
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

    public:
        statement_jump(std::string name_1,std::string name_2)
            :symbol(name_1)
            ,symbol_2(name_2)
        {}
        
        statement_jump(std::string name_1)
            :symbol(name_1)
            ,symbol_2(" ")
        {}    
        
        statement_jump(std::string name_1,NodePtr _arg)
            : expr(_arg)
            , symbol(name_1)
            , symbol_2(" ")
            
        {}

        std::string name = "statement_jump";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
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
                 */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
    //               | RETURN ';'
    //               | RETURN EXPR ';'
                
                
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"return ";
                
                if(expr != NULL){
                    
                    expr->translate(dst,context);
                }
                
                dst<<"\n";

        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            if(expr != NULL) {
                                     
                expr->compile(dst, context); // prints out an immediate or a register value
                                    
                dst<<"\taddi\t$2,$0,"<<context.expr_result<<"\n";
        

            }
            if(symbol == "return"){
                // push stack
                dst<<"\taddu\t$sp,$fp,$0\n"; // restore sp
                dst<<"\tlw\t$fp,4($sp)\n";
                dst<<"\taddiu\t$sp,$sp,"<<context.stack_size*4<<"\n";
                dst<<"\tj\t$31\n\tnop\n"; // jump to return addr
            }
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

    public:
        statement_iteration(std::string name,NodePtr _arg1,NodePtr _arg2)
            : expr(_arg1)
            , statement(_arg2)
            , statement_expr(NULL)
            , symbol(name)
            , symbol_1(" ")
            , statement_expr_rep(NULL)
        {}
        statement_iteration(std::string name,NodePtr _arg1,std::string name_1,NodePtr _arg2)
            : expr(_arg2)
            , statement(_arg1)
            , statement_expr(NULL)
            , symbol(name)
            , symbol_1(name_1)
            , statement_expr_rep(NULL)
        {}
        statement_iteration(std::string name,NodePtr _arg,NodePtr _arg1,NodePtr _arg2)
            :expr(NULL)
            ,statement(_arg2)
            ,statement_expr(_arg)
            ,symbol(name)
            ,symbol_1(" ")    
            ,statement_expr_rep(_arg1)

        {} 
        statement_iteration(std::string name,NodePtr _arg,NodePtr _arg1,NodePtr _arg2,NodePtr _arg3)
            :expr(_arg3)
            ,statement(_arg2)
            ,statement_expr(_arg)
            ,symbol(name)
            ,symbol_1(" ")
            ,statement_expr_rep(_arg1)
        {}

        std::string name = "statement_iteration";
   
        virtual void PrettyPrint(std::ostream &dst) const override
        {
        /*
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
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {

    //STATEMENT_ITERATION : WHILE L_BRACKET EXPR R_BRACKET STATEMENT
    //                    | DO STATEMENT WHILE L_BRACKET EXPR R_BRACKET ';' 
    //                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR R_BRACKET STATEMENT
    //                    | FOR L_BRACKET STATEMENT_EXPR STATEMENT_EXPR EXPR R_BRACKET STATEMEN
            
            if(symbol == "while"){

                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"while (";
             
                expr->translate(dst,context);
             
                dst<<"):"<<std::endl;
             
                context.indent++;
             
                statement->translate(dst,context);
             
                context.indent--;
            }
            
             
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
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
        
    public:
        statement_selection(std::string name,NodePtr _arg,NodePtr _arg1)
            :expr(_arg)
            ,statement(_arg1)
            ,statement_rep(NULL)
            ,symbol_1(name)
            ,symbol_2(" ")
        {}
        
        statement_selection(std::string name,NodePtr _arg,NodePtr _arg1,std::string name1 , NodePtr _arg2)
            :expr(_arg)
            ,statement(_arg1)
            ,statement_rep(_arg2)
            ,symbol_1(name)
            ,symbol_2(name1)
        {}

        std::string name = "statement_selection";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            /*
            if(symbol_2 == NULL){
                symbol_1->PrettyPrint(dst);
                dst<<'(';
                expr->PrettyPrint(dst);
                dst<<')';
                statement->PrettyPrint(dst);
            }
            else{
                dst<<"if ";
                dst<<'(';
                expr->PrettyPrint(dst);
                dst<<')';
                statement->PrettyPrint(dst);
                symbol_2->PrettyPrint(dst);
                statement_rep--PrettyPrint(dst);
            
            }  
            */
            dst<<"AST Node: "<<name<<" does not yet support PrettyPrint function"<<std::endl;
            exit(1);         
  
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(symbol_1 == "if"){
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                dst<<"if(";
                expr->translate(dst,context);
                dst<<")"<<":"<<std::endl;
                
                context.indent++;
                statement->translate(dst,context);
                context.indent--;
            }
            if(symbol_2 == "else"){
                
                for(int i=0; i<context.indent ; i++){
                    dst<<"\t";
                }
                
                dst<<"else:"<<std::endl;
                context.indent++;
                statement_rep->translate(dst,context);
                context.indent--;
            }           
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};


#endif
