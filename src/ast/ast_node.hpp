#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <vector>

#include <memory>

// abstarct class for any node of the AST
class Node;

struct CompileContext{

};

struct TranslateContext{
    int indent;
    std::vector<std::string> globalVar;
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