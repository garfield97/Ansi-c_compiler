#ifndef ast_Translation_Unit_hpp
#define ast_Translation_Unit_hpp

#include "ast_node.hpp"


class Translation_Unit
    : public Node
{
private:
    NodePtr declaration;
    NodePtr UT_next;
protected:
    Translation_Unit(NodePtr _arg1 = null, NodePtr _arg2)
        : UT_next(_arg1)
        , declaration(_arg2)
    {}
public:
 
    virtual void PrettyPrint() const override
    {
        if(UT_next != null) UT_next->print();
        declaration->print();
    }

    virtual void toPYPY() const override{

    }

    virtual void renderASM() const override{

    }
};



#endif
