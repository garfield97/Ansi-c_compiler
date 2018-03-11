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
            if(reg_free[i] == true && ( i >= 8u && i <= 15u ) ){ // register to be saved by calling function
                reg_free[i] = false;
                return i;
            }
        }

        // no free reg found in 8-15
        /*
        //give one in range 16-23
        for(int i=0; i<32; i++){
            if(reg_free[i] == true && ( i >= 16 && i <= 23 ) ){ // register to be saved by called function
                    // check for unsaved regs
                reg_free[i] = false;
                return i;
            }
        }
        */

        // all reg occupied
        free_up_reg();  // free up a reg from $8-$15
        return get_free_reg(); // now get a free reg - guaranteed
    }

    uint reg_counter;
    void free_up_reg(){
        uint s_pos;

        // search through map to find variable stored in the reg to be replaced
        for(std::map<std::string, binding>::iterator it= scopes[scope_index].begin(); it !=scopes[scope_index].end(); ++it){
            if(it->second.reg_ID == reg_counter){ // replace this variable
                // store it back to stack
                s_pos = it->second.stack_position;

                //un-assign the reg
                it->second.reg_ID = 33;

                reg_free[it->second.reg_ID] = true;
                break;
            } 
        }

        // store variable in reg back onto stack
        dstStream<<"\tsw\t$"<<reg_counter<<","<<s_pos*4<<"($fp)";

        if(++reg_counter == 16u) reg_counter = 8u; // loop back for next replacement

    }

    bool update_variable(){ // return true when given a new reg - i.e. loaded from the stack // only for variables

        uint local = scopes[scope_index][expr_result].reg_ID; // store reg ID locally
            
        if(local == 33){    // unassigned -
            local = get_free_reg();
            scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
            dstStream<<"\tlw\t$"<<local<<","<<scopes[scope_index][expr_result].stack_position*4<<"($sp)\n";
            return true;
        }    

        return false;   // no update made
    }


    uint set_literal_reg(){ // give a register to literal value for temperorary usage later

        uint local = get_free_reg(); //  assign literal a register
        dstStream<<"\taddi\t$"<<local<<",$0,"<<expr_result<<'\n';
        return local;
        
    }
    
    
    
    
    
    std::string expr_result_reg; // deal with recursion and passing registers
    uint erv_flag;
    bool erv_top;

    bool set_am_i_top(){
        if(erv_top == false){
            erv_top = true; // a node is top
            return true;
        }
        return false;

    }

    void check_am_i_top(){
        if(erv_top == true){
            erv_top = false;
            erv_flag = 0;
        }
    }

    std::string get_erv_reg(){ // retruns reg which current evaluation is using - format [0-9]+ 

            std::string result;

            if(erv_flag == 0){ // base case of an expression             
                // store first operand of RHS into temp reg
                if(regex_match(expr_result, reNum)){ // literal int
                    result = std::to_string( get_free_reg() ); // find a free reg - format [0-9]+
                    UNARY_UPDATE(); // check for unary opr
                    dstStream<<"\taddi\t"<<"$"<<result<<",$0,"<<expr_result<<'\n';  
                }
                else{   // variable
                    if(update_variable()){  // isn't stored in a reg already
                        // load from stack
                        dstStream<<"\tlw\t"<<"$"<<scopes[scope_index][expr_result].reg_ID<<","<<scopes[scope_index][expr_result].stack_position*4<<"($sp)"<<std::endl;   
                    }
                    result = std::to_string(scopes[scope_index][expr_result].reg_ID);
                    dstStream<<"\tadd\t"<<"$"<<result<<",$0,$"<<scopes[scope_index][expr_result].reg_ID<<'\n'; // move from assigned reg into expr res reg
                }

            }
            else{ // returning to another expression above in the tree
                int reg;
                sscanf(expr_result_reg.c_str(),"$%d", &reg); //strip $ from string

                result = std::to_string(reg);

                erv_flag--;

            }

            return result;

    }

    void set_erv_reg(std::string ri){
        if(erv_flag == 0) expr_result = "$"+ri; // base case - append to $ for top case
        else expr_result = ri; // pass through to upper level
     
        expr_result_reg = expr_result;
        erv_flag++;
    }
  
    void reset_erv(){ // pops last reg from erv and stores it intop expr_result
        // used when need to clear erv - finsihed evaluating an expr - if not cleared correctly yet - will ensure a clear

        if(erv_flag > 0u){ // should be 1 whenever called..

            expr_result = expr_result_reg; // store // return value

            erv_flag = 0u;   // reset
        }       
    }
    

    // used to check if variable is a literal - set in int main
    std::regex reNum;
    std::regex reChar;
    std::regex is_reg;
    std::regex reFloat;

    uint stack_size;                // dealing with stack pointer

    std::vector<std::map<std::string, binding> > scopes; // vector of bindings for each scope
    // map - variable name to register number
    uint scope_index;       // use to access orrect map in vector


    std::string current_func;
    std::string tmp_v;              // used to transfer variable name across nodes

    std::string expr_result;        // literal values
    LITERAL_TYPE expr_primary_type;

    bool UNARY_OP_MINUS_CHECK;
    void UNARY_OP_MINUS_UPDATE(){
        expr_result = "-" + expr_result;
        UNARY_OP_MINUS_CHECK = false;
    }

    void UNARY_UPDATE(){
        if(UNARY_OP_MINUS_CHECK) UNARY_OP_MINUS_UPDATE();
    }


};


#endif