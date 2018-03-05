#ifndef ast_node_hpp
#define ast_node_hpp

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>

#include <memory>

// abstarct class for any node of the AST
class Node;

struct CompileContext{
    uint32_t register_file [32];
    std::vector<uint32_t> v_address_space;
    std::vector<uint32_t> stack;
    std::vector<std::string> global_var;
    bool global_scope;
    std::string current_func;
    std::string tmp_v;  // used to transfer variable name across nodes
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