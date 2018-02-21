#ifndef ast_Translation_Unit_hpp
#define ast_Translation_Unit_hpp

#include "ast_node.hpp"


class Translation_Unit
    : public Node
{
private:
    NodePtr TU_next;
    NodePtr declaration;
protected:
    Translation_Unit(NodePtr _arg1)
        : TU_next(null)
        , declaration(_arg1)
    {}
    Translation_Unit(NodePtr _arg1, NodePtr _arg2)
        : TU_next(_arg1)
        , declaration(_arg2)
    {}
public:
 
    virtual void PrettyPrint(std::ostream &dst) const override
    {
        if(UT_next != null) TU_next->print();
        declaration->print();
    }

    virtual void toPYPY(std::ostream &dst) const override{

    }

    virtual void renderASM(std::ostream &dst) const override{

    }
};



#endif
