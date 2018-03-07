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

struct CompileContext{
    uint32_t register_file [32];    // register file

    uint stack_size;                // dealing with stack pointer

    std::vector<std::map<std::string, uint> > scopes; // vector of bindings for each scope
                                                      // map - variable name to register number

    bool global_scope;              // determine whether to add to global scope 

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
    
};


#endif