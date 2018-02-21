#ifndef ast_program_hpp
#define ast_program_Unit_hpp

#include "ast_node.hpp"


class Program
    : public Node
{
private:
    NodePtr next;
    NodePtr declaration;
protected:
    Program(NodePtr _arg1)
        : next(null)
        , declaration(_arg1)
    {}
    Program(NodePtr _arg1, NodePtr _arg2)
        : next(_arg1)
        , declaration(_arg2)
    {}
public:
 
    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(next != null) next->print();
        declaration->print();
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }
};



#endif
