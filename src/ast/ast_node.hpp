#ifndef ast_node_hpp
#define ast_node_hpp

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

// abstarct class for any node of the AST
class Node;

struct binding{
    uint reg_ID;        // register number
    std::string type;   // C type being stored
    uint stack_position;
};

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
            if(it->second.reg_ID == reg_counter){
                s_pos = it->second.stack_position;
                break;
            } 
        }

        // store variable in reg back onto stack
        dstStream<<"\tsw\t$"<<reg_counter<<","<<s_pos*4<<"($fp)";

        if(++reg_counter == 16u) reg_counter = 8u; // loop back for next replacement

    }

    
    bool update_variable(){ // return true when given a new reg - i.e. loaded from the stack

        uint local = scopes[scope_index][expr_result].reg_ID; // store current variable in tmp
            
        if(local == 33){    // unassigned
            local = get_free_reg();
            scopes[scope_index][expr_result].reg_ID = local; //updating the binding stored in our vectors of map-> no more updates to reg_assign
            return true;
        }    

        return false;   // no update made
    }
    

    // used to check if variable is a literal
    std::regex reNum;
    std::regex reChar;
    std::regex is_reg;

    uint stack_size;                // dealing with stack pointer

    std::vector<std::map<std::string, binding> > scopes; // vector of bindings for each scope
                                                      // map - variable name to register number
    uint scope_index;       // use to access orrect map in vector

    std::string current_func;
    std::string tmp_v;              // used to transfer variable name across nodes
    std::string expr_result;        // literal values
};




struct TranslateContext{
    int indent;
    std::vector<std::string> globalVar;
    bool function_dec;
    std::string tmp_v;
    bool main_exists;
};

typedef const Node* NodePtr;

class Node
{
public:
    std::string name;

    virtual ~Node()
    {}

    // print out AST
    virtual void PrettyPrint(std::ostream &dst) const =0;

    //! Tell and expression to print itself to the given stream
    virtual void translate(std::ostream &dst, TranslateContext &context) const =0;

    //! Evaluate the tree using the given mapping of variables to numbers
    virtual void compile(std::ostream &dst, CompileContext &context) const =0;
    
    void push_stack(std::ostream &dst, CompileContext &context) const{
        
        
        dst<<"\t"<<"addiu"<<"\t"<<"$sp,$sp,-4"<<'\n';
        
        for(uint i = 1; i <= context.stack_size ; i++){ 
            dst<<"\tlw\t$15,"<<i*4+4<<"($sp)"<<'\n';                                        //permanently assign register 15 as temporary stack shifting storage
            dst<<"\tsw\t$15,"<<i*4<<"($sp)"<<'\n';
        }
        dst<<"\taddu\t$fp,$sp,$0"<<std::endl;                       //outside of forloop
        context.stack_size++;
    }
    
};






#endif