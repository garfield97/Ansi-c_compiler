#ifndef ast_expression_hpp
#define ast_expression_hpp

#include "ast_node.hpp"

class expr : public Node {
    // EXPR : EXPR_ASSIGNMENT
    //      | EXPR ',' EXPR_ASSIGNMENT
    private:
        NodePtr rec;
        NodePtr assignment;
    public:
        expr(NodePtr _arg1, NodePtr _arg2)
            : rec(_arg1)
            , assignment(_arg2)
        {}
    public:
    
        std::string name = "expr";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<" , ";
            assignment->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {   
            rec->translate(dst, context);
            dst<<" , ";
            assignment->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
        
            rec->compile(dst,context);
            assignment->compile(dst,context);
            
        }
};

class expr_conditional : public Node {
    //EXPR_CONDITIONAL : EXPR_LOGIC_OR
    //                 | EXPR_LOGIC_OR '?' EXPR ':' EXPR_CONDITIONAL
    private:
        NodePtr eConditional;
        NodePtr expr;
        NodePtr eLOR;
    public:
        expr_conditional(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
            : eConditional(_arg3)
            , expr(_arg2)
            , eLOR(_arg1)
        {}
    public:
    
        std::string name = "expr_conditional";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            // EXPR_LOGIC_OR '?' EXPR ':' EXPR_CONDITIONAL
            eLOR->PrettyPrint(dst);
            dst<<'?';
            expr->PrettyPrint(dst);
            dst<<':';
            eConditional->PrettyPrint(dst);

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

class expr_assignment : public Node {
    //EXPR_ASSIGNMENT : EXPR_CONDITIONAL
    //                | EXPR_UNARY OPR_ASSIGNMENT EXPR_ASSIGNMENT
    private:
        NodePtr unary;
        NodePtr opr;
        NodePtr exp;
    public:
        expr_assignment(NodePtr _arg1, NodePtr _arg2, NodePtr _arg3)
            : unary(_arg1)
            , opr(_arg2)
            , exp(_arg3)
        {}
    public:

        std::string name = "expr_assignment";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            // EXPR_UNARY OPR_ASSIGNMENT EXPR_ASSIGNMENT
            unary->PrettyPrint(dst);
            opr->PrettyPrint(dst);
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            unary->translate(dst, context);
            opr->translate(dst, context);
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            unary->compile(dst,context);                                //Find the name of the variable stored in expression_result
            bool update = context.update_variable(); // true if loaded from stack                     
            
            binding tmp = context.scopes[context.scope_index][context.expr_result];
                
            if(update) dst<<"\tlw\t"<<"$"<<tmp.reg_ID<<","<<tmp.stack_position*4<<"($sp)"<<std::endl; //this loads from stack into register.      
            
            // This only supports normal assign currently. assuming that opr_assignment is '= ' 
            
            // evaluates RHS
            exp->compile(dst,context);
            update = context.update_variable(); // don't care about result
            

            context.reset_erv(); // check if an expression was evaluated - stores reg ID into expr_result

            if( regex_match(context.expr_result,context.is_reg) ){
                dst<<"\tadd\t"<<"$"<<tmp.reg_ID<<",$0,"<<context.expr_result<<std::endl;
            }
            else { // literal                   
            dst<<"\taddi\t"<<"$"<<tmp.reg_ID<<",$0,"<<context.expr_result<<std::endl;    // move results into assignment register. Mips Mov STORE RESULT FROM EXPRESSION INTO REGISTER THAT WAS ASSIGNED - Good comment OK                    
            }   
             
                
        
        }
};

class opr_assignment: public Node {
    private:
        std::string opr;
    public:
        opr_assignment(std::string _opr)
            : opr(_opr)
        {}
        
        std::string name = "opr_assignment";

        std::string getOpr(){
            return opr;
        }
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            dst<<opr<<' ';
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<opr;
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class expr_logic_or : public Node {
    // EXPR_LOGIC_OR : EXPR_LOGIC_AND
    //               | EXPR_LOGIC_OR OP_LOR EXPR_LOGIC_AND
    private:
        NodePtr rec;
        NodePtr exp;
        
    public:
        expr_logic_or(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}       
    public:
    
        std::string name = "expr_logic_or";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<"|| ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<" or ";
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class expr_logic_and : public Node {
    //EXPR_LOGIC_AND : EXPR_INCLUSIVE_OR  
    //               | EXPR_LOGIC_AND OP_LAND EXPR_INCLUSIVE_OR
    private:
        NodePtr rec;
        NodePtr exp;
        
    public:
        expr_logic_and(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}
    public:

        std::string name = "expr_logic_and";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<"&& ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<" and ";
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class expr_inclusive_or : public Node {
    //EXPR_INCLUSIVE_OR : EXPR_XOR 
    //                  | EXPR_INCLUSIVE_OR OP_BOR EXPR_XOR
    private:
        NodePtr rec;
        NodePtr exp;
        
    public:
        expr_inclusive_or(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

    public:
    
        std::string name = "expr_inclusive_or";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<"| ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<"|";
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class expr_xor : public Node {
    //EXPR_XOR : EXPR_AND
    //         | EXPR_XOR OP_BXOR EXPR_AND
    private:
        NodePtr rec;
        NodePtr exp;
        
    public:
        expr_xor(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

    public:

         std::string name = "expr_xor";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<"^ ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<"^";
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class expr_and : public Node {
    //EXPR_AND : EXPR_EQUALITY
    //         | EXPR_AND OP_BAND EXPR_EQUALITY
    private:
        NodePtr rec;
        NodePtr exp;
        
    public:
        expr_and(NodePtr _rec, NodePtr _exp)
            : rec(_rec)
            , exp(_exp)
        {}

    public:

         std::string name = "expr_and";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<"& ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<"&";
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
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
        
    public:
        expr_equality(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

    public:
    
        std::string name = "expr_equality";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<op<<" ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<op;
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
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
        
    public:
        expr_relational(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

    public:

         std::string name = "expr_relational";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<op<<" ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<op;
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
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
        
    public:
        expr_shift(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

    public:

         std::string name = "expr_shift";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<op<<" ";
            exp->PrettyPrint(dst);
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

class expr_add : public Node {
    //EXPR_ADD : EXPR_MUL
    //         | EXPR_ADD OP_PLUS EXPR_MUL
    //         | EXPR_ADD OP_MINUS EXPR_MUL
    private:
        NodePtr rec;
        std::string op;
        NodePtr exp;
        
    public:
        expr_add(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}
        
    public:

         std::string name = "expr_add";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<op<<" ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<op;
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            bool top = context.set_am_i_top();     // check if i'm top node;

            rec->compile(dst, context); // store variable into expression result

            std::string temp_register = context.get_erv_reg(); // obtian relevant reg_ID - format [0-9]+
            

            exp->compile(dst,context); // compile right most term
            
            context.UNARY_UPDATE();

            // check result type
            std::string mul_reg;
            if(regex_match(context.expr_result, context.reNum)) mul_reg = context.expr_result;
            // reg
            else if(regex_match(context.expr_result, context.is_reg)) mul_reg = context.expr_result;
            // variable
            else{
                if(context.update_variable()){ } // is stored in a reg already 
                mul_reg = "$" + std::to_string(context.scopes[context.scope_index][context.expr_result].reg_ID);
            }

            
            if(op == "+"){
            
                if(regex_match(context.expr_result, context.reNum)){ // literal
                                               
                    if(context.expr_primary_type == UI){
                        dst<<"\taddiu\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n';      //implementation for unsigned litearal
                    }
                    else{
                        dst<<"\taddi\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n';       //signed literal
                    }   
                }
                else{   // variable

                    if(context.expr_primary_type == UI){
                        dst<<"\taddu\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n'; // register addition -> storage  unsigned register add
                    }
                    else{
                        dst<<"\tadd\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n'; // register addition -> storage  signed register add
                    }
                }            
            }
            
            else{ // subtraction
                
                if(regex_match(context.expr_result, context.reNum)){ // literal
                    
                    if(context.expr_primary_type == UI){
                        dst<<"\tsubiu\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n';  //unsigned subtract - immediate
                    }
                    else{
                        dst<<"\tsubi\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n';   //signed subtract - immediate
                    }
                    
                }
                else{   // variable

                    if(context.expr_primary_type == UI){
                        dst<<"\tsubu\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n'; // register addition -> storage 
                    }
                    
                    else{
                        dst<<"\tsub\t"<<"$"<<temp_register<<",$"<<temp_register<<","<<mul_reg<<'\n'; // register addition -> storage 
                    }
               }
            }
            
            if(!top)context.set_erv_reg(temp_register); // to pass back reg used to store result // leaves at 1 on top case
            else context.check_am_i_top(temp_register);        
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
        
    public:
        expr_mul(NodePtr _rec, std::string _op, NodePtr _exp)
            : rec(_rec)
            , op(_op)
            , exp(_exp)
        {}

    public:

         std::string name = "expr_mul";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            rec->PrettyPrint(dst);
            dst<<op<<" ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            rec->translate(dst, context);
            dst<<op;
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            bool top = context.set_am_i_top();     // check if i'm top node;

            // EXPR_MUL
            rec->compile(dst, context); // store variable into expression result
            std::string temp_register = context.get_erv_reg(); // obtian relevant reg_ID - format [0-9]+
       
            // EXPR_CAST
            exp->compile(dst,context); // compile right most term // expr_result has expr_cast value
            context.UNARY_UPDATE();

            // check literal
            uint cast_reg;
            if(regex_match(context.expr_result, context.reNum)) cast_reg = context.set_literal_reg();
            // variable
            else{
                cast_reg = context.scopes[context.scope_index][context.expr_result].reg_ID;

                if(context.update_variable()){  // is a vairbale stored in a reg already
                    dst<<"\tlw\t"<<"$"<<cast_reg<<","<<context.scopes[context.scope_index][context.expr_result].stack_position*4<<"($sp)"<<std::endl;   
                }
            }
            

            if(op == "*"){
                if(context.expr_primary_type == UI){
                    dst<<"\tmultu\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // unsigned mul
                }
                else{
                    dst<<"\tmult\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // mul
                }

                dst<<"\tmflo\t$"<<temp_register<<"\n";
            }

            if(op == "/"){ //LO has quotient
                if(context.expr_primary_type == UI){
                    dst<<"\tdivu\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // unsigned div
                }
                else{
                    dst<<"\tdiv\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // div
                }

                dst<<"\tmflo\t$"<<temp_register<<"\n"; 
            }

            if(op == "%"){ //HI has remainder
                if(context.expr_primary_type == UI){
                    dst<<"\tdivu\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // unsigned div
                }
                else{
                    dst<<"\tdiv\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // div
                }

                dst<<"\tmfhi\t$"<<temp_register<<"\n"; 
            }
          

            if(!top)context.set_erv_reg(temp_register); // to pass back reg used to store result // leaves at 1 on top case
            else context.check_am_i_top(temp_register);

        }
};

class expr_cast : public Node {
    //EXPR_CAST : EXPR_UNARY
    //          | L_BRACKET NAME_TYPE R_BRACKET EXPR_CAST
    private:
        NodePtr rec;
        NodePtr exp;
        
    public:
        expr_cast(NodePtr _exp, NodePtr _rec)
            : rec(_rec)
            , exp(_exp)
        {}

    public:

         std::string name = "expr_cast";
    
        virtual void PrettyPrint(std::ostream &dst) const override
        {
            // L_BRACKET NAME_TYPE R_BRACKET EXPR_CAST
            dst<<" ( ";
            exp->PrettyPrint(dst);
            dst<<" ) ";
            rec->PrettyPrint(dst);
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

class expr_unary : public Node {
    //EXPR_UNARY : EXPR_POSTFIX 
    //           | OP_INC EXPR_UNARY
    //           | OP_DEC EXPR_UNARY
    //           | OPR_UNARY EXPR_CAST
    //           | SIZEOF EXPR_UNARY
    //           | SIZEOF L_BRACKET NAME_TYPE R_BRACKET  
    private:
        std::string terminal;
		NodePtr O_U;
        NodePtr exp;
        
    public:
        expr_unary(std::string _terminal, NodePtr _exp)
            : terminal(_terminal)
			, O_U(NULL)
			, exp(_exp)
        {}
		
		expr_unary(NodePtr _arg1, NodePtr _exp)
            : terminal(" ")
            , O_U(_arg1)
			, exp(_exp)
        {}

    public:
    
        std::string name = "expr_unary";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(terminal != " "){
                dst<<terminal<<" ";
            }
			
            if(O_U != NULL) O_U->PrettyPrint(dst);
			
            if(exp->name == "name_type"){
				dst<<" ( ";
				exp->PrettyPrint(dst);
				dst<<" ) ";
			}
			else exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(O_U != NULL) O_U->translate(dst, context);
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            if(O_U != NULL){
            

                O_U->compile(dst,context);
                std::string tmp_op = context.expr_result;
                                
                exp->compile(dst,context);
                
                if(tmp_op == "-"){
                    
                    if(regex_match(context.expr_result,context.reNum)){          //literal matching to append - in front
                        
                        context.UNARY_OP_MINUS_CHECK = true;                    
                    }
                        
                
                    else{
                    
                        if(context.update_variable()){}
                            
                        dst<<"\tsub\t$"<<context.scopes[context.scope_index][context.expr_result].reg_ID<<",$0,$"<<context.scopes[context.scope_index][context.expr_result].reg_ID<<'\n';

                
                    }
                }
    
                if(tmp_op == "+"){
                
                }
                
                
            
            }
            
            

        }
};

class opr_unary: public Node {
    private:
        std::string opr;
    public:
        opr_unary(std::string _opr)
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

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            dst<<opr;
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            context.expr_result = opr;
        }
};

class arg_expr_list : public Node {
    //ARG_EXPR_LIST : EXPR_ASSIGNMENT
    //              | ARG_EXPR_LIST ',' EXPR_ASSIGNMENT
    private:
        NodePtr next;
        NodePtr exp;
        
    public:
        arg_expr_list(NodePtr _next, NodePtr _exp)
            : next(_next)
            , exp(_exp)
        {}

    public:
    
        std::string name = "arg_expr_list";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            next->PrettyPrint(dst);
            dst<<" , ";
            exp->PrettyPrint(dst);
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            next->translate(dst, context);
            dst<<",";
            exp->translate(dst, context);
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
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
        
    public:
        expr_postfix(NodePtr _next, NodePtr _exp)// LS exp RS
            : next(_next)
            , opr(" ")
            , id(" ")
            , bracket(false)
            , exp(_exp)
        {}

        expr_postfix(NodePtr _next, bool _bracket)// L R
            : next(_next)
            , opr(" ")
            , id(" ")
            , bracket(true)
            , exp(NULL)
        {}

        expr_postfix(NodePtr _next, bool _bracket, NodePtr _exp)// L AEL R
            : next(_next)
            , opr(" ")
            , id(" ")
            , bracket(true)
            , exp(_exp)
        {}

        expr_postfix(NodePtr _next, std::string _opr, std::string _id) // OP_PTR(->) '.'
            : next(_next)
            , opr(_opr)
            , id(_id)
            , bracket(false)
            , exp(NULL)
        {}

        expr_postfix(NodePtr _next, std::string _opr) // OP_INC OP_DEC
            : next(_next)
            , opr(_opr)
            , id(" ")
            , bracket(false)
            , exp(NULL)
        {}   

    public:
    
        std::string name = "expr_postfix";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            next->PrettyPrint(dst);
            if(exp != NULL && !bracket){
                // EXPR_POSTFIX L_SQUARE EXPR R_SQUARE
                dst<<" [ ";
                exp->PrettyPrint(dst);
                dst<<" ] ";
            }
            else if(bracket){
                // EXPR_POSTFIX L_BRACKET R_BRACKET
                // EXPR_POSTFIX L_BRACKET ARG_EXPR_LIST R_BRACKET
                dst<<" ( ";
                if(exp != NULL) exp->PrettyPrint(dst);
                dst<<" ) ";
            }
            else if(id != " "){
                //             | EXPR_POSTFIX '.' IDENTIFIER
                //             | EXPR_POSTFIX OP_PTR IDENTIFIER // OP_PTR == ->
                dst<<opr<<" "<<id;
            }
            else{
                //             | EXPR_POSTFIX OP_INC
                //             | EXPR_POSTFIX OP_DEC
                dst<<opr<<" ";
            }
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            //             | EXPR_POSTFIX L_BRACKET ARG_EXPR_LIST R_BRACKET
            next->translate(dst, context);
            dst<<"(";
            if(exp != NULL) exp->translate(dst, context);
            dst<<")";
        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            dst<<"AST Node: "<<name<<" does not yet support compile function"<<std::endl;
            exit(1);
        }
};

class expr_primary : public Node {
    //EXPR_PRIMARY : IDENTIFIER
    //             | INT_C 
    //             | UNSIGNED_C 
    //             | LONG_C 
    //             | UNSIGNED_LONG_C
    //             | CHARACTER_C
    //             | STRING_LITERAL
    //             | L_BRACKET EXPR R_BRACKET
    private:
        NodePtr exp;
        std::string Sval;        
        int Ival;
        unsigned int UIval;
        long int LIval;
        unsigned long ULval;
        char Cval;
        bool Sbool;
        bool Ibool;
        bool UIbool;
        bool LIbool;
        bool ULbool;
        bool Cbool;
        
    public:
        expr_primary(std::string _val)
            : exp(NULL)
            , Sval(_val)
            , Sbool(true)
            , Ibool(false)
            , UIbool(false)
            , LIbool(false)
            , ULbool(false)
            , Cbool(false)
        {}

        expr_primary(int _val)
            : exp(NULL)
            , Ival(_val)
            , Sbool(false)
            , Ibool(true)
            , UIbool(false)
            , LIbool(false)
            , ULbool(false)
            , Cbool(false)
        {}

        expr_primary(unsigned int _val)
            : exp(NULL)
            , UIval(_val)
            , Sbool(false)
            , Ibool(false)
            , UIbool(true)
            , LIbool(false)
            , ULbool(false)
            , Cbool(false)
        {}

        expr_primary(long int _val)
            : exp(NULL)
            , LIval(_val)
            , Sbool(false)
            , Ibool(false)
            , UIbool(false)
            , LIbool(true)
            , ULbool(false)
            , Cbool(false)
        {}

        expr_primary(unsigned long _val)
            : exp(NULL)
            , ULval(_val)
            , Sbool(false)
            , Ibool(false)
            , UIbool(false)
            , LIbool(false)
            , ULbool(true)
            , Cbool(false)
        {}

        expr_primary(char _val)
           : exp(NULL)
            , Cval(_val)
            , Sbool(false)
            , Ibool(false)
            , UIbool(false)
            , LIbool(false)
            , ULbool(false)
            , Cbool(true)
        {}

        expr_primary(NodePtr _exp)
            : exp(_exp)
            , Sbool(false)
            , Ibool(false)
            , UIbool(false)
            , LIbool(false)
            , ULbool(false)
            , Cbool(false)
        {}

    public:
    
        std::string name = "expr_primary";

        virtual void PrettyPrint(std::ostream &dst) const override
        {
            if(exp != NULL){
                dst<<" ( ";
                exp->PrettyPrint(dst);
                dst<<" ) ";
            }
            else if(Sbool)  dst<<Sval<<" ";
            else if(Ibool)  dst<<Ival<<" ";
            else if(UIbool) dst<<UIval<<" ";
            else if(LIbool) dst<<LIval<<" ";
            else if(ULbool) dst<<ULval<<" ";
            else if(Cbool)  dst<<Cval<<" ";
        }

        virtual void translate(std::ostream &dst, TranslateContext &context) const override
        {
            if(Sbool)  dst<<Sval;
            else if(Ibool)  dst<<Ival;
            else if(UIbool) dst<<UIval;
            else if(LIbool) dst<<LIval;
            else if(ULbool) dst<<ULval;
            else if(Cbool)  dst<<Cval;
            else if(exp != NULL){
                dst<<"(";
                exp->translate(dst, context);
                dst<<")";
            }

        }

        virtual void compile(std::ostream &dst, CompileContext &context) const override
        {
            if(Sbool){
                context.expr_result = Sval;
                context.expr_primary_type = S; // enum val
            } 
            else if(Ibool){
                context.expr_result = std::to_string(Ival);
                context.expr_primary_type = I;
            }
            else if(UIbool){
                context.expr_result = std::to_string(UIval);
                context.expr_primary_type = UI;
            }
            else if(LIbool){
                context.expr_result = std::to_string(LIval);
                context.expr_primary_type = LI;
            }
            else if(ULbool){
                context.expr_result = std::to_string(ULval);
                context.expr_primary_type = UL;
            } 
            else if(Cbool){
                context.expr_result = std::to_string(Cval);
                context.expr_primary_type = C;
            }
            else if(exp != NULL){
                exp->compile(dst, context);
            }        
            
        }
};

#endif