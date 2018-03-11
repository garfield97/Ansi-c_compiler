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
                break;
            } 
        }

        // store variable in reg back onto stack
        dstStream<<"\tsw\t$"<<reg_counter<<","<<s_pos*4<<"($fp)";

        if(++reg_counter == 16u) reg_counter = 8u; // loop back for next replacement

    }

    bool update_variable(){ // return true when given a new reg - i.e. loaded from the stack

        uint local = scopes[scope_index][expr_result].reg_ID; // store reg ID locally
            
        if(local == 33){    // unassigned
            local = get_free_reg();
            scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
            dstStream<<"\tlw\t$"<<local<<","<<scopes[scope_index][expr_result].stack_position*4<<"($sp)\n";
            return true;
        }    

        return false;   // no update made
    }


    std::vector<std::string> expr_result_vector; // deal with recursion and passing registers
    uint erv_index;

    std::string get_erv_reg(){ // retruns reg which current evaluation is using - format [0-9]+ 

            std::string result;

            if(erv_index == 0){ // base case of an expression

                result = std::to_string( get_free_reg() ); // find a free reg - format [0-9]+
                
                // store first operand of RHS into temp reg

                if(regex_match(expr_result, reNum)){ // literal int
                    dstStream<<"\taddi\t"<<"$"<<result<<",$0,"<<expr_result<<'\n';  
                }
                else{   // variable
                    if(update_variable()){  // isn't stored in a reg already
                        // load from stack
                        dstStream<<"\tlw\t"<<"$"<<scopes[scope_index][expr_result].reg_ID<<","<<scopes[scope_index][expr_result].stack_position*4<<"($sp)"<<std::endl;   
                    }
                    dstStream<<"\tadd\t"<<"$"<<result<<",$0,$"<<scopes[scope_index][expr_result].reg_ID<<'\n'; // move from assigned reg into expr res reg
                }

            }
            else{ // returning to another expression above in the tree
                int reg;
                sscanf(expr_result_vector[erv_index-1].c_str(),"$%d", &reg); //strip $ from string

                result = std::to_string(reg);

                // remove reg from erv - also free up

                reg_free[reg] = true; // frees up reg for use

                expr_result_vector.pop_back(); // remove off the tmp reg stack
                erv_index--;

            }

            return result;

    }

    void set_erv_reg(std::string ri){
        if(erv_index == 0) expr_result = "$"+ri; // base case - append to $ for top case
        else expr_result = ri; // pass through to upper level
        
        expr_result_vector.push_back(expr_result); // store to erv stack
        erv_index++;
    }
  
    void reset_erv(){ // pops last reg from erv and stores it intop expr_result
        // used when need to clear erv - finsihed evaluating an expr - if not cleared correctly yet - will ensure a clear

        if(erv_index > 0u){ // should be 1 whenever called..

            expr_result = expr_result_vector[erv_index - 1]; // store // return value

            // clear stack
            for(uint i = 0u; i<expr_result_vector.size(); i++){
                expr_result_vector.pop_back();
            }

            erv_index = 0u;   // reset
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
};


#endif