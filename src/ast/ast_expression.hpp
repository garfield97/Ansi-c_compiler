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
            //EXPR and EXPR_CONDITONAL send result to same register - fine as only one code path executed

            bool top = context.am_i_top();     // check if i'm top node;

            std::string zero = context.makeName("zero");
            std::string skip = context.makeName("skip");

            // evaluate expr_logic_or
            eLOR->compile(dst, context); // store variable into expression result
            std::string cmp_reg = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets

            // beq zero - zero
            dst<<"\tbeq\t"<<"$"<<cmp_reg<<",$0,"<<"$"<<zero<<"\n";


            context.err_overide = true;
            context.err_overide_reg = context.get_free_reg(); // used to force same reg for both expr compile


            // ne zero code
                // free up for compile call
                bool t = context.err_top, b = context.err_bottom; // save state locally
                std::string r = context.expr_result_reg;
                context.err_top = false;
                context.err_bottom = false;

                expr->compile(dst,context); // compile right most term 
                context.UNARY_UPDATE();

                context.err_top = t;        // restore state
                context.err_bottom = b;
                context.expr_result_reg = r;

                // get register to pass back
                uint extracted_reg = context.extract_expr_reg();
                dst<<"\tmove\t"<<"$"<<context.err_overide_reg<<",$"<<extracted_reg<<"\n";
            
                // b skip
                dst<<"\tb\t"<<"$"<<skip<<"\n";
            
            

            // zero:
            dst<<"$"<<zero<<":\n";
            // zero code
                // free up for compile call
                t = context.err_top, b = context.err_bottom; // save state locally
                r = context.expr_result_reg;
                context.err_top = false;
                context.err_bottom = false;

                eConditional->compile(dst,context); // compile right most term 
                context.UNARY_UPDATE();

                context.err_top = t;        // restore state
                context.err_bottom = b;
                context.expr_result_reg = r;

                // get register to pass back
                extracted_reg = context.extract_expr_reg();
                dst<<"\tmove\t"<<"$"<<context.err_overide_reg<<",$"<<extracted_reg<<"\n";


            // skip:
            dst<<"$"<<skip<<":\n";

            

            context.err_overide = false; // resume normal expr_reg allocation

            if(top) context.i_am_top(std::to_string(context.err_overide_reg) ); // send to above node that isnt recursive
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
            bool top = context.am_i_top();     // check if i'm top node;
            if(top){
                context.assigning = true;
                context.assign_reg_set = false;
            }
            
            context.expr_primary_global_var = false; // reset

            // EXPR_UNARY
            unary->compile(dst,context); // store variable into expression result
            std::string var_assigned = context.expr_result;

            // check for global
            bool global_var = context.expr_primary_global_var;
            context.expr_primary_global_var = false; // reset


            std::string unary_reg;
            if(!global_var) unary_reg = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets
            else{
                var_assigned = context.global_expr_result;
                unary_reg = std::to_string(context.get_free_reg());
            }
            
            binding tmp = context.scopes[context.scope_index][context.expr_result];


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            context.type_cast = false;
            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;

            // check for cast
            if(context.type_cast){
                context.type_cast = false;
                tmp.type = context.expr_cast_type;
            }


            // get RH term register
            uint exp_reg = context.extract_expr_reg();


            // sets tmp_v with operator
            opr->compile(dst,context);
            std::string mode = context.tmp_v;
            // get type
            std::string type = tmp.type;


            if(mode == "="){ // normal assign
                dst<<"\tmove\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;

            }
            if(mode == "*="){ // mul assign
                if(type == "unsigned int"){
                    dst<<"\tmultu\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                else{
                    dst<<"\tmult\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                dst<<"\tmflo\t"<<"$"<<unary_reg<<"\n";

            }
            if(mode == "/="){ // div assign
                if(type == "unsigned int"){
                    dst<<"\tdivu\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                else{
                    dst<<"\tdiv\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                dst<<"\tmflo\t"<<"$"<<unary_reg<<"\n";

            }
            if(mode == "%="){ // mod assign
                 if(type == "unsigned int"){
                    dst<<"\tdivu\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                else{
                    dst<<"\tdiv\t"<<"$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                dst<<"\tmfhi\t"<<"$"<<unary_reg<<"\n";
               
            }
            if(mode == "+="){ // add assign
                if(type == "unsigned int"){
                    dst<<"\taddu\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                else{
                    dst<<"\tadd\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }

            }
            if(mode == "-="){ // sub assign
                if(type == "unsigned int"){
                    dst<<"\tsubu\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                else{
                    dst<<"\tsub\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
            }
            if(mode == "<<="){ // left assign
                dst<<"\tsllv\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;

            }
            if(mode == ">>="){ // right assign
                if(type == "unsigned int"){
                    dst<<"\tsrlv\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }
                else{
                    dst<<"\tsrav\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
                }

            }
            if(mode == "&="){ // and assign
                dst<<"\tand\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
            }
            if(mode == "^="){ // xor assign
                dst<<"\txor\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
            }
            if(mode == "|="){ // or assign
                dst<<"\tor\t"<<"$"<<unary_reg<<",$"<<unary_reg<<",$"<<exp_reg<<std::endl;
            }


            if(!global_var){
                dst<<"\tsw\t"<<"$"<<unary_reg<<","<<tmp.stack_position*4<<"($sp)"<<std::endl;  // get s_pos from top of function  
            }
            else{   // writing to global
                // load address
                uint addr_reg = context.get_free_reg();
                dst<<"\tlui\t"<<"$"<<addr_reg<<",%hi("<<var_assigned<<")\n";

                dst<<"\tsw\t"<<"$"<<unary_reg<<",%lo("<<var_assigned<<")($"<<addr_reg<<")\n";
            }

            // update reg for variable binding
            sscanf(unary_reg.c_str(),"$%d", &tmp.reg_ID);
            context.scopes[context.scope_index][var_assigned] = tmp;


            if(top){    
                context.i_am_top(unary_reg); // send to above node that isnt recursive
                context.assigning = false;
                context.assign_reg_set = false;
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
            context.tmp_v = opr;
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
            bool top = context.am_i_top();     // check if i'm top node;


            std::string zero = context.makeName("LOR");
            std::string one = context.makeName("LOR");
            std::string end = context.makeName("LOR");



            // EXPR_LOGIC_OR
            rec->compile(dst, context); // store variable into expression result

            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets



            //bne temp 0 - move 1
            dst<<"\tbne\t$"<<temp_register<<",$0,$"<<one<<"\n";



            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint logic_or_reg = context.extract_expr_reg();



            //beq exp 0 - move 0
            dst<<"\tbeq\t$"<<logic_or_reg<<",$0,$"<<zero<<"\n";

            //move 1
            dst<<"$"<<one<<":\n";
            dst<<"\tmove\t$"<<temp_register<<",$1\n";
            //b end
            dst<<"\tb\t$"<<end<<"\n";

            //move 0
            dst<<"$"<<zero<<":\n";
            dst<<"\tmove\t$"<<temp_register<<",$0\n";

            // end
            dst<<"$"<<end<<":\n";



            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive
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
            bool top = context.am_i_top();     // check if i'm top node;

            std::string zero = context.makeName("LAND");
            std::string skip = context.makeName("LAND");

            // EXPR_LOGIC_AND
            rec->compile(dst, context); // store variable into expression result

            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets

            // compare temp_register
            dst<<"\tbeq\t"<<"$"<<temp_register<<",$0"<<",$"<<zero<<'\n';
                //if eq branch to move 0


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint logic_and_reg = context.extract_expr_reg();


            // compare l_a_reg
            dst<<"\tbeq\t"<<"$"<<logic_and_reg<<",$0"<<",$"<<zero<<'\n';
                // if eq to branch to move 0
            // move 1
            dst<<"\tmove\t"<<"$"<<temp_register<<",$1\n";
            // branch past move 0
            dst<<"\tb\t"<<"$"<<skip<<'\n';

            //move 0
            dst<<"$"<<zero<<":\n";
            dst<<"\tmove\t"<<"$"<<temp_register<<",$0\n";

            //skip
            dst<<"$"<<skip<<":\n";

            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive
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
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_INCLUSIVE_OR
            rec->compile(dst, context); // store variable into expression result
            context.internal_expr_value = context.internal_temp_value; // get internal value form rec

            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint xor_reg = context.extract_expr_reg();


            // bitwise OR
            context.internal_expr_value |= context.internal_temp_value; 
            dst<<"\tor\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<xor_reg<<'\n';
        
            context.internal_temp_value = context.internal_expr_value; 
            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive

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
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_AND
            rec->compile(dst, context); // store variable into expression result
            context.internal_expr_value = context.internal_temp_value; // get internal value form rec

            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint and_reg = context.extract_expr_reg();


            // bitwise OR
            context.internal_expr_value ^= context.internal_temp_value;
            dst<<"\txor\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<and_reg<<'\n';
        
            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive

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
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_EQUALITY
            rec->compile(dst, context); // store variable into expression result
            
            context.internal_expr_value = context.internal_temp_value;
            
            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint eq_reg = context.extract_expr_reg();


            // bitwise AND
            context.internal_expr_value &= context.internal_temp_value;
            dst<<"\tand\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<eq_reg<<'\n';
        
            context.internal_temp_value = context.internal_expr_value;

            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive

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
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_EQUALITY
            rec->compile(dst, context); // store variable into expression result
            context.internal_expr_value = context.internal_temp_value;
            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint logic_and_reg = context.extract_expr_reg();
            if(op == "=="){
                context.internal_expr_value = context.internal_expr_value == context.internal_temp_value;
                dst<<"\tseq\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<logic_and_reg<<'\n';
            }
            
            else{
                context.internal_expr_value = context.internal_expr_value != context.internal_temp_value;
                dst<<"\tsne\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<logic_and_reg<<'\n';
            }
            
         
            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive
        
         }
};

class expr_relational : public Node {             
    //EXPR_RELATIONAL : EXPR_SHIFT                        { $$ = $1;                                }
    //                | EXPR_RELATIONAL OP_L EXPR_SHIFT   { $$ = new expr_relational($1, "<", $3);  }
    //                | EXPR_RELATIONAL OP_G EXPR_SHIFT   { $$ = new expr_relational($1, ">", $3);  }
    //                | EXPR_RELATIONAL OP_LE EXPR_SHIFT  { $$ = new expr_relational($1, "<=", $3); }
    //                | EXPR_RELATIONAL OP_GE EXPR_SHIFT  { $$ = new expr_relational($1, ">=", $3); }
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
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_SHIFT
            rec->compile(dst, context); // store variable into expression result

            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.internal_expr_value = context.internal_temp_value;
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint relational_reg = context.extract_expr_reg();

        if(context.expr_primary_type == UI){  
                if(op == "<"){
                    context.internal_expr_value = context.internal_expr_value < context.internal_temp_value;
                    dst<<"\tsltu\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
                else if(op == ">"){
                    context.internal_expr_value = context.internal_expr_value > context.internal_temp_value;
                    dst<<"\tsgtu\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
                else if(op == "<="){
                    context.internal_expr_value = context.internal_expr_value <= context.internal_temp_value;
                    dst<<"\tsleu\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
                else if(op == ">="){
                    context.internal_expr_value = context.internal_expr_value >= context.internal_temp_value;
                    dst<<"\tsgeu\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
            }
            
            else{
                if(op == "<"){
                    context.internal_expr_value = context.internal_expr_value < context.internal_temp_value;
                    dst<<"\tslt\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
                else if(op == ">"){
                    context.internal_expr_value = context.internal_expr_value > context.internal_temp_value;
                    dst<<"\tsgt\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
                else if(op == "<="){
                    context.internal_expr_value = context.internal_expr_value <= context.internal_temp_value;
                    dst<<"\tsle\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
                else if(op == ">="){
                    context.internal_expr_value = context.internal_expr_value >= context.internal_temp_value;
                    dst<<"\tsge\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<relational_reg<<'\n';
                }
            }       
 
            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive
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
      
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_SHIFT
            rec->compile(dst, context); // store variable into expression result
            context.internal_expr_value = context.internal_temp_value;
            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            uint shift_reg = context.extract_expr_reg();


            if(op == "<<"){
                context.internal_expr_value <<= context.internal_temp_value;
                dst<<"\tsllv\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<shift_reg<<'\n';
            }
            
            else{
                context.internal_expr_value >>= context.internal_temp_value;
                if(context.expr_primary_type == UI){
                    dst<<"\tsrlv\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<shift_reg<<'\n';
                }
                else{
                    dst<<"\tsrav\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<shift_reg<<'\n';
                }
            }


            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive
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
            bool top = context.am_i_top();     // check if i'm top node;

            rec->compile(dst, context); // store variable into expression result
            context.internal_expr_value = context.internal_temp_value; // get internal value form rec

            std::string temp_register = context.am_i_bottom();

            // check to free for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;
            exp->compile(dst,context); // compile right most term
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // check result type
            std::string mul_reg = std::to_string(context.extract_expr_reg());

            
            if(op == "+"){
                context.internal_expr_value += context.internal_temp_value;

                if(context.expr_primary_type == UI){
                    dst<<"\taddu\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<mul_reg<<'\n'; // register addition -> storage  unsigned register add
                }
                else{
                    dst<<"\tadd\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<mul_reg<<'\n'; // register addition -> storage  signed register add
                }         
            }
            
            else{ // subtraction
                context.internal_expr_value -= context.internal_temp_value;
                if(context.expr_primary_type == UI){
                    dst<<"\tsubu\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<mul_reg<<'\n'; // register addition -> storage 
                }
                
                else{
                    dst<<"\tsub\t"<<"$"<<temp_register<<",$"<<temp_register<<",$"<<mul_reg<<'\n'; // register addition -> storage 
                }
            }
            

            context.internal_temp_value = context.internal_expr_value; //overwrite ITV to ITV 
            if(top) context.i_am_top(temp_register); // if top after rec calls // send to above node that isnt recursive       
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
            bool top = context.am_i_top();     // check if i'm top node;

            // EXPR_MUL
            rec->compile(dst, context); // store variable into expression result

            context.internal_expr_value = context.internal_temp_value;
            
            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // check to free for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;
            exp->compile(dst,context); // compile right most term // will change reg
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // check literal
            uint cast_reg = context.extract_expr_reg();
            

            if(op == "*"){
                
                context.internal_expr_value *= context.internal_temp_value;
                if(context.expr_primary_type == UI){
                    dst<<"\tmultu\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // unsigned mul
                }
                else{
                    dst<<"\tmult\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // mul
                }

                dst<<"\tmflo\t$"<<temp_register<<"\n";
            }

            if(op == "/"){ //LO has quotient
            
                context.internal_expr_value /= context.internal_temp_value;
                if(context.expr_primary_type == UI){
                    dst<<"\tdivu\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // unsigned div
                }
                else{
                    dst<<"\tdiv\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // div
                }

                dst<<"\tmflo\t$"<<temp_register<<"\n"; 
            }

            if(op == "%"){ //HI has remainder
                
                context.internal_expr_value %= context.internal_temp_value;
                if(context.expr_primary_type == UI){
                    dst<<"\tdivu\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // unsigned div
                }
                else{
                    dst<<"\tdiv\t"<<"$"<<temp_register<<",$"<<cast_reg<<'\n'; // div
                }

                dst<<"\tmfhi\t$"<<temp_register<<"\n"; 
            }
          


            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive

        }
};

class expr_cast : public Node {
    //EXPR_CAST : EXPR_UNARY
    //          | L_BRACKET TYPE_NAME R_BRACKET EXPR_CAST
    private:
        NodePtr rec;
        NodePtr exp; //TYPE_NAME
        
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
            
            exp->compile(dst,context); //sets tmp_v to correct type to convert to (TYPE_NAME)
            context.expr_cast_type = context.tmp_v;
            context.type_cast = true;
            //check this is expr_assignment


            // Get expr result / value
            bool top = context.am_i_top();     // check if i'm top node;

            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            rec->compile(dst, context); // get value

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;

            std::string temp_register = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            if(top) context.i_am_top(temp_register); // send to above node that isnt recursive
        }
};

// sizeoff not done - come back later

/*bug for inc / dec
    - can only work for rhs terms, lhs terms don't recieve rhs inc/dec
    - very hard to determine pre inc/dec before accessing
    - better to spend dev time elsewhere
*/
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
        bool brackets;
        
    public:
        expr_unary(std::string _terminal, NodePtr _exp)
            : terminal(_terminal)
			, O_U(NULL)
			, exp(_exp)
            , brackets(false)
        {}

        expr_unary(std::string _terminal, NodePtr _exp, bool b)
            : terminal(_terminal)
			, O_U(NULL)
			, exp(_exp)
            , brackets(true)
        {}
		
		expr_unary(NodePtr _arg1, NodePtr _exp)
            : terminal(" ")
            , O_U(_arg1)
			, exp(_exp)
            , brackets(false)
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
            bool top = context.am_i_top();     // check if i'm top node;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.internal_expr_value = context.internal_temp_value;
            if(brackets) context.sizeof_type = true;

            std::string exp_reg = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // Operations

            if(terminal == "sizeof"){ 

                if(brackets){ // type //tmp_v has type 

                    uint size = 4;
                    // if tmp_v in map of custom struct
                        //add supprt later

                    // if char > -3

                    // if long long > +4

                    // if double > +4

                    // if 
                    
                    
                    dst<<"\tmove\t$"<<exp_reg<<","<<size<<std::endl;
                    context.sizeof_type = false;
                }
                else{ //expr

                }
                
            }

            // INC and DEC
            if(terminal == "++"){
                if(context.expr_primary_type == UI){
                    dst<<"\taddiu\t$"<<exp_reg<<",$"<<exp_reg<<",1"<<std::endl;
                }
                else{
                    dst<<"\taddi\t$"<<exp_reg<<",$"<<exp_reg<<",1"<<std::endl;
                }
                context.force_update_variable();
                uint local = context.scopes[context.scope_index][context.expr_result].reg_ID;
                dst<<"\tmove\t$"<<local<<",$"<<exp_reg<<"\n";
                dst<<"\tsw\t$"<<local<<","<<context.scopes[context.scope_index][context.expr_result].stack_position*4<<"($sp)\n";
            }

            if(terminal == "--"){
                if(context.expr_primary_type == UI){
                    dst<<"\tsubiu\t$"<<exp_reg<<",$"<<exp_reg<<",1"<<std::endl;
                }
                else{
                    dst<<"\taddi\t$"<<exp_reg<<",$"<<exp_reg<<",-1"<<std::endl;
                }
                context.force_update_variable();
                uint local = context.scopes[context.scope_index][context.expr_result].reg_ID;
                dst<<"\tmove\t$"<<local<<",$"<<exp_reg<<"\n";
                dst<<"\tsw\t$"<<local<<","<<context.scopes[context.scope_index][context.expr_result].stack_position*4<<"($sp)\n";
            }
                

            // OPR_UNARY
            if(O_U != NULL){
                std::string var_save = context.expr_result;

                O_U->compile(dst,context);
                std::string tmp_op = context.expr_result;

                context.expr_result = var_save;

                if(tmp_op == "-"){
                    context.internal_expr_value = -context.internal_temp_value;             
                    if(context.update_variable()){}                           
                    dst<<"\tsub\t$"<<exp_reg<<",$0,$"<<exp_reg<<'\n';              
                }
                if(tmp_op == "+"){
                
                }
                if(tmp_op == "!"){
                    if(context.update_variable()){} 
                    
                    std::string zero = context.makeName("eq0");
                    std::string skip = context.makeName("skip");

                    // check if its zero
                    dst<<"\tbeq\t$"<<exp_reg<<",$0,$"<<zero<<"\n";

                    // set to 0
                    dst<<"\taddi\t$"<<exp_reg<<",$0,0"<<'\n';
                    // branch to end
                    dst<<"\tbeq\t$"<<exp_reg<<",$0,$"<<skip<<"\n";
                    //dst<<"\tb\t$"<<skip<<":\n"<<"\tnop\n";


                    // set to 1
                    dst<<"$"<<zero<<":\n";
                    dst<<"\taddi\t$"<<exp_reg<<",$0,1"<<'\n';

                    //end
                    dst<<"$"<<skip<<":\n";


                    context.internal_expr_value = !context.internal_temp_value;                
                }
                if(tmp_op == "~"){ 
                    context.internal_expr_value = ~context.internal_temp_value;                
                    if(context.update_variable()){}                           
                    dst<<"\tnot\t$"<<exp_reg<<",$"<<exp_reg<<'\n';              
                }       
            }



            // end of operations code

            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(exp_reg); // send to above node that isnt recursive
                
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

// for functions
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
            bool top = context.am_i_top();     // check if i'm top node;

            // ARG_EXPR_LIST
            next->compile(dst, context); // store variable into expression result
            context.internal_expr_value = context.internal_temp_value;
            std::string arg_reg = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets


            // free bools for rhs
            bool t = context.err_top, b = context.err_bottom; // save state locally
            std::string r = context.expr_result_reg;
            context.err_top = false;
            context.err_bottom = false;

            exp->compile(dst,context); // compile right most term 
            context.UNARY_UPDATE();

            context.err_top = t;        // restore state
            context.err_bottom = b;
            context.expr_result_reg = r;


            // get RH term register
            //uint exp_reg = context.extract_expr_reg();





            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(arg_reg); // send to above node that isnt recursive
        }
};

// missing arrayy / functions with args and pointers
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

        expr_postfix(std::string _opr, NodePtr _next) // OP_INC OP_DEC
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
            bool top = context.am_i_top();     // check if i'm top node;
            next->compile(dst,context); // compile right most term

            context.UNARY_UPDATE();
            
            context.internal_expr_value = context.internal_temp_value;
         
            if(bracket){ // function call
                binding tmp;
                tmp.reg_ID = 33;
                tmp.stack_position = 6969;
                context.scopes[context.scope_index][context.expr_result] = tmp;
            }
            std::string exp_reg = context.am_i_bottom(); // check if bottom expr node // sets expr_result_reg if, otherwise gets
            
            // Operations

            // INC and DEC
            if(opr == "++"){
                context.force_update_variable();
                uint local = context.scopes[context.scope_index][context.expr_result].reg_ID; //get x into loca
                
                if(context.expr_primary_type == UI){
                    dst<<"\tmove\t$"<<exp_reg<<",$"<<exp_reg<<std::endl; // set y = x 
                    dst<<"\taddiu\t$"<<local<<",$"<<exp_reg<<",1\n";            // increment x by 1  
                }
                else{
                    dst<<"\tmove\t$"<<exp_reg<<",$"<<exp_reg<<std::endl;      //same as before but for signed
                    dst<<"\taddi\t$"<<local<<",$"<<exp_reg<<",1\n";                 
                }
                dst<<"\tsw\t$"<<local<<","<<context.scopes[context.scope_index][context.expr_result].stack_position*4<<"($sp)\n";   //saves values back onto stack

            }

            else if(opr == "--"){
                context.force_update_variable();
                uint local = context.scopes[context.scope_index][context.expr_result].reg_ID; //get x into loca
                
                if(context.expr_primary_type == UI){
                    dst<<"\tmove\t$"<<exp_reg<<",$"<<exp_reg<<std::endl; // set y = x 
                    dst<<"\taddiu\t$"<<local<<",$"<<exp_reg<<",-1\n";            // decrement x by 1  
                }
                else{
                    dst<<"\tmove\t$"<<exp_reg<<",$"<<exp_reg<<std::endl;      //same as before but for signed
                    dst<<"\taddi\t$"<<local<<",$"<<exp_reg<<",-1\n";                 
                }
                dst<<"\tsw\t$"<<local<<","<<context.scopes[context.scope_index][context.expr_result].stack_position*4<<"($sp)\n";   //saves values back onto stack

            }             
                        
            // function call of 0 arguments
            else if (bracket && (exp == NULL)){
                //allocate space on stack for this function
                this->push_stack(dst,context);
                uint s_pos = context.stack_size;

                // save $8-$15 to the stack
                context.save_8_15();

                uint save_size = context.stack_size;
                context.stack_size = 0;
                //next is the function name
                dst<<"\tjal\t"<<context.expr_result<<"\n\tnop\n";

                dst<<"\tsw\t$2,"<<s_pos*4<<"($fp)\n";
                context.stack_size = save_size;

                // restore $8-15
                context.restore_8_15();

                dst<<"\tmove\t$"<<exp_reg<<",$2\n";

            }

            // end of operations code

            context.internal_temp_value = context.internal_expr_value;
            if(top) context.i_am_top(exp_reg); // send to above node that isnt recursive
        }
};

// missing float
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
                if(!context.scopes[0][Sval].is_global){
                    context.expr_result = Sval;
                    context.internal_temp_value = context.scopes[context.scope_index][Sval].internal_value;
                    context.internal_expr_value = context.internal_temp_value;
                    context.set_expr_result_type();
                }
                else{
                    context.expr_primary_global_var = true;
                    context.global_expr_result = Sval;
                    context.expr_result = Sval;
                    uint g_reg = context.extract_expr_reg();
                    context.expr_result = "$"+std::to_string(g_reg);

                    context.internal_temp_value = context.scopes[0][Sval].internal_value;
                    context.internal_expr_value = context.internal_temp_value;

                }
            } 
            else if(Ibool){
                context.expr_result = std::to_string(Ival);
                context.internal_temp_value = context.internal_expr_value = Ival;
                context.expr_primary_type = I;
            }
            else if(UIbool){
                context.expr_result = std::to_string(UIval);
                context.internal_temp_value = context.internal_expr_value = UIval;
                context.expr_primary_type = UI;
            }
            else if(LIbool){
                context.expr_result = std::to_string(LIval);
                context.internal_temp_value = context.internal_expr_value = LIval;
                context.expr_primary_type = LI;
            }
            else if(ULbool){
                context.expr_result = std::to_string(ULval);
                context.internal_temp_value = context.internal_expr_value = ULval;
                context.expr_primary_type = UL;
            } 
            else if(Cbool){
                context.expr_result = std::to_string(Cval);
                context.internal_temp_value = context.internal_expr_value = Cval;
                context.expr_primary_type = C;
            }
            else if(exp != NULL){
                exp->compile(dst, context);
            }        
            
        }
};

#endif