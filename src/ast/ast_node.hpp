#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <map>

#include <memory>

// abstarct class for any node of the AST
class Node;

typedef const Node* NodePtr;

class Node
{
public:
    virtual ~Node()
    {}

    // print out AST
    virtual void PrettyPrint() const =0;

    //! Tell and expression to print itself to the given stream
    virtual void toPY() const =0;

    //! Evaluate the tree using the given mapping of variables to numbers
    virtual void renderASM() const =0;
    
};


#endif