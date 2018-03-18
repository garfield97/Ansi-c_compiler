#ifndef compile_context_hpp
#define compile_context_hpp

#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <stdio.h>
#include <fstream>
#include <memory>

extern std::ofstream dstStream;

struct binding{
    uint reg_ID;        // register number
    std::string type;   // C type being stored
    uint stack_position;
    long int internal_value;
};

// use to identify tpye a primary expression is
typedef enum{
    S,   // string
    I,   // integer
    UI,  // unsigned integer
    LI,  // long integer
    UL,  // unsigned long
    C,   // character
    F    // float
} LITERAL_TYPE;

struct CompileContext{

    bool reg_free[32];              // check if reg available
    uint get_free_reg(){

        for(uint i=0u; i<32u; i++){
            if(reg_free[i] == true && ( i >= 8u && i <= 14u ) && not_in_err_stack(i) ){ // register to be saved by calling function
                reg_free[i] = false;

                        // EXPR_ASSIGNMENT 
                if(assigning && !assign_reg_set){ // need to set assign reg
                    assign_reg = i;
                    assign_reg_set = true;
                }

                return i;
            }
        }

        // all reg occupied
        free_up_reg();  // free up a reg from $8-$15
        return get_free_reg(); // now get a free reg - guaranteed
    }

    bool assigning;  // when true - do not free up assign reg
    bool assign_reg_set;
    uint assign_reg; // this cannot be freed

    uint reg_counter;

    void free_up_reg(){
        uint s_pos;
        bool found = false;
        if(reg_counter == assign_reg && assigning) ++reg_counter; // avoid freeing assign_reg

        // first save any varubale stored in reg_counter
        // search through variables in current scope
        for(std::map<std::string, binding>::iterator it= scopes[scope_index].begin(); it !=scopes[scope_index].end(); ++it){
            if(it->second.reg_ID == reg_counter){ // replace this variable
                // save locally
                s_pos = it->second.stack_position;
                found = true;

                // update variable to not ina register
                it->second.reg_ID = 33;
            } 
        }

        if(found){
            // store variable in reg back onto stack
            dstStream<<"\tsw\t$"<<reg_counter<<","<<s_pos*4<<"($fp)";
        }

        // now free it
        reg_free[reg_counter] = true; // free the reg

        if(++reg_counter == 15u) reg_counter = 8u; // loop back for next replacement

    }

    bool update_variable(){ // return true when given a new reg - i.e. loaded from the stack // only for variables

        uint local = scopes[scope_index][expr_result].reg_ID; // store reg ID locally

        uint s_pos = scopes[scope_index][expr_result].stack_position;

        if(!declaring && (s_pos == 0) && (scope_index > 0) && no_match() ){   //var not in this scope
            binding tmp = out_of_scope(scope_index-1);
            local = tmp.reg_ID;
            s_pos = tmp.stack_position;
        }
            
        if(local == 33){    // unassigned -
            local = get_free_reg();
            scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
            scopes[scope_index][expr_result].stack_position = s_pos;
            dstStream<<"\tlw\t$"<<local<<","<<s_pos*4<<"($sp)\n";
            return true;
        }    

        return false;   // no update made
    }

    void force_update_variable(){ // return true when given a new reg - i.e. loaded from the stack // only for variables

        uint s_pos = scopes[scope_index][expr_result].stack_position;
        if(!declaring && (s_pos == 0) && (scope_index > 0) && no_match() ){   //var not in this scope
            s_pos = out_of_scope(scope_index-1).stack_position;
        }
                            
        uint local = get_free_reg();
        scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
        scopes[scope_index][expr_result].stack_position = s_pos;
        dstStream<<"\tlw\t$"<<local<<","<<s_pos*4<<"($sp)\n";

    }

    bool declaring;

    binding out_of_scope(uint index){
        binding res;
        //iterate through map from scope above
        for(std::map<std::string, binding>::iterator it= scopes[index].begin(); it !=scopes[index].end(); ++it){
            if(expr_result == it->first){
                res = it->second;
                return res;
            }
        }

        if(index == 0) std::exit(EXIT_FAILURE); // safety for invalid C
        res = out_of_scope(index-1);
        return res;
    }

    uint set_literal_reg(){ // give a register to literal value for temperorary usage later

        uint local = get_free_reg(); //  assign literal a register
        dstStream<<"\taddi\t$"<<local<<",$0,"<<expr_result<<'\n';
        return local;
        
    }
    

    uint extract_expr_reg(){
        uint reg;
        //literal
        if(regex_match(expr_result, reNum)) reg = set_literal_reg();
        // check if reg
        else if(regex_match(expr_result, is_reg)) sscanf(expr_result.c_str(),"$%d", &reg);
        // variable
        else{
            uint reg_save = scopes[scope_index][expr_result].reg_ID;

            force_update_variable();  // force the variable a new reg
            reg = scopes[scope_index][expr_result].reg_ID;
            dstStream<<"\tlw\t"<<"$"<<reg<<","<<scopes[scope_index][expr_result].stack_position*4<<"($sp)"<<std::endl;   

            scopes[scope_index][expr_result].reg_ID = reg_save; //restore binding
        }

        return reg;
    }


  

    std::string expr_result_reg; // deal with recursion and passing registers
    bool err_top;
    bool err_bottom;
    std::vector<uint> err_stack;
    bool err_overide;
    uint err_overide_reg;
    bool sizeof_type;

    bool not_in_err_stack(uint x){
        for(uint i = 0; i < err_stack.size(); ++i){
            if(err_stack[i] == x){
                return false; // found this value in the stack
            }
        }

        return true; // free to use this value - its not been preserved
    }

    std::string am_i_bottom(){ // returns reg as num in string
        // not conditional
        if(!err_overide){
            if(err_bottom == false){
                err_bottom = true; //

                uint err_stack_reg;

                if(regex_match(expr_result, reNum)){ // literal int
                    err_stack_reg = get_free_reg();
                    expr_result_reg = std::to_string( err_stack_reg ); // find a free reg - format [0-9]+
                    UNARY_UPDATE(); // check for unary opr
                    dstStream<<"\taddi\t"<<"$"<<expr_result_reg<<",$0,"<<expr_result<<'\n';  
                }
                else if(sizeof_type){ // size of type
                    err_stack_reg = get_free_reg();
                    expr_result_reg = std::to_string( err_stack_reg );
                }
                else{   // variable
                    uint reg_save = scopes[scope_index][expr_result].reg_ID;
                    force_update_variable();  // froce a new reg
                        // load from stack
                    dstStream<<"\tlw\t"<<"$"<<scopes[scope_index][expr_result].reg_ID<<","<<scopes[scope_index][expr_result].stack_position*4<<"($sp)"<<std::endl;   
                    
                    err_stack_reg = scopes[scope_index][expr_result].reg_ID;
                    expr_result_reg = std::to_string(scopes[scope_index][expr_result].reg_ID);
                    dstStream<<"\tadd\t"<<"$"<<expr_result_reg<<",$0,$"<<scopes[scope_index][expr_result].reg_ID<<'\n'; // move from assigned reg into expr res reg
                
                    scopes[scope_index][expr_result].reg_ID = reg_save; //restore binding
                }


                err_stack.push_back(err_stack_reg); // save reg to preserved stack
            }
        }
        
        else{
            expr_result_reg = std::to_string(err_overide_reg); // use set reg
        }

        return expr_result_reg;

    }

    bool am_i_top(){
        if(err_top == false){
            err_top = true; // a node is top
            return true;
        }
        return false;

    }

    void i_am_top(std::string r){
        if(err_top == true){
            err_top = false;
            err_bottom = false;
            err_stack.pop_back();
            expr_result = "$"+r;
        }
    }


    std::string break_label;
    std::string continue_label;
    std::string switch_end_label;

    long int internal_expr_value;
    long int internal_temp_value;


    // used to check if variable is a literal - set in int main
    std::regex reNum;
    std::regex reChar;
    std::regex is_reg;
    std::regex reFloat;

    bool no_match(){
        if(
            !regex_match(expr_result, reNum)
            &&
            !regex_match(expr_result, reChar)
            &&
            !regex_match(expr_result, is_reg)
            &&
            !regex_match(expr_result, reFloat)
        ){
            return true;        // is a variable
        }
        return false; // not a variable
    }

    uint stack_size;                // dealing with stack pointer

    uint declarations;      // keep track of amount of declarations inside a scope

    std::vector<std::map<std::string, binding> > scopes; // vector of bindings for each scope
    // map - variable name to register number
    uint scope_index;       // use to access orrect map in vector


    std::string current_func;
    std::string tmp_v;              // used to transfer variable name across nodes


    std::string expr_result;
    LITERAL_TYPE expr_primary_type;
    std::string expr_cast_type;
    bool type_cast;

    void set_expr_result_type(){        // used in declare
            std::string type = scopes[scope_index][expr_result].type;
            if(type == "int"){
                expr_primary_type = I;
            }
            else if(type == "unsigned int"){
                expr_primary_type = UI;
            }
            else if(type == "long int"){
                expr_primary_type = LI;
            }
            else if(type == "unsigned long"){
                expr_primary_type = UL;
            } 
            else if(type == "char"){
                expr_primary_type = C;
            }
    }


    int makeNameUnq;

    std::string makeName(std::string base){
        return base+std::to_string(makeNameUnq++);
    }
    const char* makeNameChar(const char* base){ // use to create new file
        std::string res = base+std::to_string(makeNameUnq++);
        const char* ret = res.c_str() + '.' + 's';
        return ret;
    }


    bool UNARY_OP_MINUS_CHECK;
    void UNARY_OP_MINUS_UPDATE(){
        expr_result = "-" + expr_result;
        UNARY_OP_MINUS_CHECK = false;
    }

    void UNARY_UPDATE(){
        if(UNARY_OP_MINUS_CHECK) UNARY_OP_MINUS_UPDATE();
    }

    void function_end(){
        stack_size = 0;
        
    }


    void save_8_15(){
        dstStream<<"\t"<<"addiu"<<"\t"<<"$sp,$sp,-32"<<'\n';
        
        for(uint i = 1; i <= stack_size ; i++){ 
            dstStream<<"\tlw\t$15,"<<i*4+32<<"($sp)"<<'\n'; 
            dstStream<<"\tsw\t$15,"<<i*4<<"($sp)"<<'\n';
        }                    

        for(uint i = 0; i < 8; i++){
            //store to stack
            dstStream<<"\tsw\t$"<<i+8<<","<<(i+stack_size+1)*4<<"($sp)"<<'\n';
        }

        dstStream<<"\taddu\t$fp,$sp,$0"<<std::endl; 
        stack_size += 8;
    }

    void restore_8_15(){

        for(uint i = 0; i < 8; i++){
            //load from stack
            dstStream<<"\tlw\t$"<<15-i<<","<<(stack_size-i)*4<<"($sp)"<<'\n';
        }

        // shift up
        for(uint i = 1; i <= stack_size-8 ; i++){ 
            dstStream<<"\tlw\t$15,"<<i*4<<"($sp)"<<'\n'; 
            dstStream<<"\tsw\t$15,"<<i*4+32<<"($sp)"<<'\n';
        }                   

        dstStream<<"\t"<<"addiu"<<"\t"<<"$sp,$sp,32"<<'\n';
        stack_size -= 8;
        dstStream<<"\taddu\t$fp,$sp,$0"<<std::endl;  
    }

};


#endif