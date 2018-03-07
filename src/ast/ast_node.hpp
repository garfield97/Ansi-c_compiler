#ifndef ast_node_hpp
#define ast_node_hpp

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <memory>

// abstarct class for any node of the AST
class Node;

struct binding{
    uint reg_ID;        // register number
    std::string type;   // C type being stored
    uint stack_position;
};

struct CompileContext{
    bool reg_free[32];              // check if reg available
    int get_free_reg(){
        for(int i=0; i<32; i++){
            if(reg_free[i] == true && ( i >= 8 && i <= 15 ) ){
                    // check for unsaved regs
                return i;
            }
        }
        return 33;      // no free reg - need to backup - implement later
    }


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